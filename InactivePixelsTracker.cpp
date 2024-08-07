#include "InactivePixelsTracker.hpp"
#include <utility>

bool InactivePixelsTracker::hasInactiveSide(const cv::Mat& gray, Side side, int threshold) {
    const int stripWidth = 2;
    std::atomic<bool> inactive(true);

    cv::parallel_for_(cv::Range(0, stripWidth), [&](const cv::Range& range) {
        for (int j = range.start; j < range.end; j++) {
            if (!inactive.load()) return;

            for (int i = 0; i < (side == Side::TOP || side == Side::BOTTOM ? gray.cols : gray.rows); i++) {
                uchar pixelValue;
                switch (side) {
                    case Side::TOP:
                        pixelValue = gray.at<uchar>(j, i);
                        break;
                    case Side::BOTTOM:
                        pixelValue = gray.at<uchar>(gray.rows - 1 - j, i);
                        break;
                    case Side::LEFT:
                        pixelValue = gray.at<uchar>(i, j);
                        break;
                    case Side::RIGHT:
                        pixelValue = gray.at<uchar>(i, gray.cols - 1 - j);
                        break;
                }
                if (pixelValue > threshold) {
                    inactive.store(false);
                    return;
                }
            }
        }
    });

    return inactive.load();
}

bool InactivePixelsTracker::hasInactivePixels(const cv::Mat& frame, int threshold = 1, int innerThreshold = 50) {
    cv::Mat gray;
    cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);
    cv::equalizeHist(gray, gray);
    cv::threshold(gray, gray, threshold, 255, cv::THRESH_BINARY);

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
    else if (currentFrame >= m_end + bufferFrames + 1 && m_start != -1) {
        m_inactivePixels.push_back(std::make_pair(m_start, m_end));
        m_start = -1;
        m_end = -1;
    }
}

void InactivePixelsTracker::appendInactivePixels(std::list<std::pair<int, int>> ipfs) {
    this->m_inactivePixels.insert(this->m_inactivePixels.end(), ipfs.begin(), ipfs.end());
}
