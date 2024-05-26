#include "InactivePixelsTracker.hpp"
#include <thread>
#include <atomic>
#include <utility>


bool InactivePixelsTracker::hasLetterbox(const cv::Mat& frame, int threshold = 0) {
    cv::Mat binary;
    cv::cvtColor(frame, binary, cv::COLOR_BGR2GRAY);
    cv::threshold(binary, binary, threshold, 255, cv::THRESH_BINARY);
    std::atomic<bool> letterbox(false);
    cv::parallel_for_(cv::Range(0, binary.cols), [&](const cv::Range& range) {
        for (int x = range.start; x < range.end; x++) {
            if (letterbox.load()) return;
            if (*(binary.ptr<uchar>(0) + x) > 0 || *(binary.ptr<uchar>(binary.rows - 1) + x) > 0) {
                letterbox.store(true);
                return;
            }
        }
    });
    return !letterbox.load();
}

bool InactivePixelsTracker::hasPillarbox(const cv::Mat& frame, int threshold = 0) {
    cv::Mat binary;
    cv::cvtColor(frame, binary, cv::COLOR_BGR2GRAY);
    cv::threshold(binary, binary, threshold, 255, cv::THRESH_BINARY);
    std::atomic<bool> pillarbox(false);
    cv::parallel_for_(cv::Range(0, binary.rows), [&](const cv::Range& range) {
        for (int y = range.start; y < range.end; y++) {
            if (pillarbox.load()) return;
            const uchar* row_ptr = binary.ptr<uchar>(y);
            if (*(row_ptr) > 0 || *(row_ptr + binary.cols - 1) > 0) {
                pillarbox.store(true);
                return;
            }
        }
    });
    return !pillarbox.load();
}

bool InactivePixelsTracker::hasInactivePixels(const cv::Mat& frame, int threshold = 0, int innerThreshold = 30) {
    std::atomic<bool> found(false);

    std::thread pillarboxThread([&]() {
        if (!found.load() && hasPillarbox(frame, threshold)) {
            found.store(true);
        }
    });
    std::thread letterboxThread([&]() {
        if (!found.load() && hasLetterbox(frame, threshold)) {
            found.store(true);
        }
    });

    pillarboxThread.join();
    letterboxThread.join();

    if (found.load()) {
        cv::Scalar mean, stddev;
        cv::meanStdDev(frame, mean, stddev);
        if (mean[0] < innerThreshold && stddev[0] < innerThreshold) {
            return false;
        }
        return true;
    }
    return false;
}

void InactivePixelsTracker::printInactivePixels() {
	for (auto& pair : m_inactivePixels)
		std::cout << "Inactive pixels between: " << pair.first << " and " << pair.second << std::endl;
}

void InactivePixelsTracker::recordInactivePixels(const cv::Mat& frame, const cv::VideoCapture& cap) {
    int currentFrame = cap.get(cv::CAP_PROP_POS_FRAMES);

    if (hasInactivePixels(frame)) {
        if (m_start == -1)
            m_start = currentFrame;
//        std::cout << "Inactive pixels at: " << currentFrame << std::endl;
        m_end = currentFrame;
    }
    else if (currentFrame == m_end + 1 && m_start != -1){
//        std::cout << "Letterboxing found between frames: " << m_start << " and " << m_end << std::endl;
		m_inactivePixels.push_back(std::make_pair(m_start, m_end));
        m_start = -1;
        m_end = -1;
    }
}
