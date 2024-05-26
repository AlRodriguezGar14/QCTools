#include "InactivePixelsTracker.hpp"
#include <thread>
#include <atomic> // to access data from multiple threads. Similar to mutex but more efficient

bool InactivePixelsTracker::hasLetterbox(const cv::Mat& frame, int threshold = 0) {
    std::atomic<bool> letterbox(false);
    cv::parallel_for_(cv::Range(0, frame.cols), [&](const cv::Range& range) {
        for (int x = range.start; x < range.end; x++) {
            if (letterbox.load()) return;
            if (*(frame.ptr<uchar>(0) + x) > threshold || *(frame.ptr<uchar>(frame.rows - 1) + x) > threshold) {
                letterbox.store(true);
                return;
            }
        }
    });
    return !letterbox.load(); // load() reads the value of the atomic variable
}

bool InactivePixelsTracker::hasPillarbox(const cv::Mat& frame, int threshold = 0) {
    std::atomic<bool> pillarbox(false);
    cv::parallel_for_(cv::Range(0, frame.rows), [&](const cv::Range& range) {
        for (int y = range.start; y < range.end; y++) {
            if (pillarbox.load()) return;
            const uchar* row_ptr = frame.ptr<uchar>(y);
            if (*(row_ptr) > threshold || *(row_ptr + frame.cols - 1) > threshold) {
                pillarbox.store(true);
                return;
            }
        }
    });
    return !pillarbox.load();
}


bool InactivePixelsTracker::hasInactivePixels(const cv::Mat& frame, int threshold = 0, int innerThreshold = 30) {
    cv::Mat gray;
    if (frame.channels() == 3) {
        cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);
    } else {
        gray = frame;
    }

    bool hasPillarboxResult, hasLetterboxResult;

    // Launch hasPillarbox and hasLetterbox in separate threads
    std::thread pillarboxThread([&]() {
        hasPillarboxResult = hasPillarbox(gray, threshold);
    });
    std::thread letterboxThread([&]() {
        hasLetterboxResult = hasLetterbox(gray, threshold);
    });

    // Wait for both threads to finish
    pillarboxThread.join();
    letterboxThread.join();

    if (hasPillarboxResult || hasLetterboxResult) {
        cv::Scalar mean, stddev;
        cv::meanStdDev(gray, mean, stddev);
        if (mean[0] < innerThreshold && stddev[0] < innerThreshold) {
            return false;
        }
        return true;
    }
    return false;
}

void InactivePixelsTracker::recordInactivePixels(const cv::Mat& frame, const cv::VideoCapture& cap) {
    int currentFrame = cap.get(cv::CAP_PROP_POS_FRAMES);

    if (hasInactivePixels(frame)) {
        if (m_start == -1)
            m_start = currentFrame;
        std::cout << "Inactive pixels at: " << currentFrame << std::endl;
        m_inactivePixels.push_back(currentFrame);
        m_end = currentFrame;
    }
    else if (currentFrame == m_end + 1){
        std::cout << "Letterboxing found between frames: " << m_start << " and " << m_end << std::endl;
        m_start = -1;
        m_end = -1;
    }
}
