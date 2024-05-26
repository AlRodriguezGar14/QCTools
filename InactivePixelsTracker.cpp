#include "InactivePixelsTracker.hpp"

bool InactivePixelsTracker::hasPillarbox(const cv::Mat& frame, int threshold = 0) {
    for (int y = 0; y < frame.rows; y++) {
        const uchar* row_ptr = frame.ptr<uchar>(y);
        if (row_ptr[0] > threshold || row_ptr[frame.cols - 1] > threshold)
            return false;
    }
    return true;
}

bool InactivePixelsTracker::hasLetterbox(const cv::Mat& frame, int threshold = 0) {
    const uchar* top_row_ptr = frame.ptr<uchar>(0);
    const uchar* bottom_row_ptr = frame.ptr<uchar>(frame.rows - 1);
    for (int x = 0; x < frame.cols; x++) {
        if (top_row_ptr[x] > threshold || bottom_row_ptr[x] > threshold)
            return false;
    }
    return true;
}

bool InactivePixelsTracker::hasInactivePixels(const cv::Mat& frame, int threshold = 0, int innerThreshold = 30) {
    cv::Mat gray;
    if (frame.channels() == 3) {
        cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);
    } else {
        gray = frame;
    }
    cv::normalize(gray, gray, 0, 200, cv::NORM_MINMAX);

    if (hasLetterbox(gray, threshold) || hasPillarbox(gray, threshold)) {
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
