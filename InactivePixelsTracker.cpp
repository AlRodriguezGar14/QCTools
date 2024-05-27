#include "InactivePixelsTracker.hpp"
#include <utility>

bool InactivePixelsTracker::hasInactiveSide(const cv::Mat& gray, Side side, int threshold) {
    std::atomic<bool> inactive(true);
    cv::parallel_for_(cv::Range(0, (side == Side::TOP || side == Side::BOTTOM) ? gray.cols : gray.rows), [&](const cv::Range& range) {
        for (int i = range.start; i < range.end; i++) {
            if (!inactive.load()) return;
            uchar pixelValue;
            switch (side) {
                case Side::TOP:
                    pixelValue = *(gray.ptr<uchar>(0) + i);
                    break;
                case Side::BOTTOM:
                    pixelValue = *(gray.ptr<uchar>(gray.rows - 1) + i);
                    break;
                case Side::LEFT:
                    pixelValue = *(gray.ptr<uchar>(i));
                    break;
                case Side::RIGHT:
                    pixelValue = *(gray.ptr<uchar>(i) + gray.cols - 1);
                    break;
            }
            if (pixelValue > threshold) {
                inactive.store(false);
                return;
            }
        }
    });
    return inactive.load();
}

bool InactivePixelsTracker::hasInactivePixels(const cv::Mat& frame, int threshold, int innerThreshold) {
    cv::Mat gray;
    cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);
	cv::equalizeHist(gray, gray);
	cv::threshold(gray, gray, 3, 255, cv::THRESH_BINARY);

    if (hasInactiveSide(gray, Side::TOP, threshold) ||
        hasInactiveSide(gray, Side::BOTTOM, threshold) ||
        hasInactiveSide(gray, Side::LEFT, threshold) ||
        hasInactiveSide(gray, Side::RIGHT, threshold)) {

        cv::Scalar mean, stddev;
        cv::meanStdDev(frame, mean, stddev);
        if (mean[0] < innerThreshold && stddev[0] < innerThreshold) {
            return false;
        }
        return true;
    }
    return false;
}

void InactivePixelsTracker::recordInactivePixels(const cv::Mat& frame, const cv::VideoCapture& cap) {
    int currentFrame = cap.get(cv::CAP_PROP_POS_FRAMES);
    const int bufferFrames = 4;

    if (hasInactivePixels(frame)) {
        if (m_start == -1)
            m_start = currentFrame;
        m_end = currentFrame;
    }
    else if (currentFrame >= m_end + bufferFrames + 1 && m_start != -1){
        m_inactivePixels.push_back(std::make_pair(m_start, m_end));
        m_start = -1;
        m_end = -1;
    }
}
void InactivePixelsTracker::printInactivePixels() {
    for (auto& pair : m_inactivePixels)
        std::cout << "Inactive pixels between: " << pair.first << " and " << pair.second << std::endl;
}
