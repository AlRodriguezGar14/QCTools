#include "duplicateFramesTracker.hpp"
#include <utility>

// TODO: Potentially build SSIM method (Structural Similarity Index) to compare frames

//bool DuplicateFramesTracker::findDuplicates(cv::Mat current, cv::Mat previous) {
//    if (previous.empty())
//        return false;
//
//    cv::Mat diff;
//    cv::subtract(current, previous, diff);
//    cv::cvtColor(diff, diff, cv::COLOR_BGR2GRAY);
//
//    int nonZero = cv::countNonZero(diff); // nonZero return: Number of non-zero pixels in the image
//    int nonZeroThreshold = current.total() * 0.005; // 0.5% of the total pixels in the image
//
//    return nonZero < nonZeroThreshold;
//}

bool DuplicateFramesTracker::findDuplicates(cv::Mat current, cv::Mat previous) {
    if (previous.empty())
        return false;

    cv::Mat diff;
    cv::subtract(current, previous, diff);
    cv::cvtColor(diff, diff, cv::COLOR_BGR2GRAY);

    int nonZero = cv::countNonZero(diff); // nonZero return: Number of non-zero pixels in the image
    int nonZeroThreshold = current.total() * 0.005; // 0.5% of the total pixels in the image

    if (nonZero >= nonZeroThreshold) {
        return false;
    }

    // YCrCb color space lets the program compare luminance
    cv::Mat ycrcbCurrent, ycrcbPrevious;
    cv::cvtColor(current, ycrcbCurrent, cv::COLOR_BGR2YCrCb);
    cv::cvtColor(previous, ycrcbPrevious, cv::COLOR_BGR2YCrCb);

    // Split the YCrCb images into their channels, as luminance is 0
    std::vector<cv::Mat> channelsCurrent, channelsPrevious;
    cv::split(ycrcbCurrent, channelsCurrent);
    cv::split(ycrcbPrevious, channelsPrevious);

    // Calculate the absolute difference between the Y channels (luminance) of the two frames
    cv::Mat luminanceDiff;
    cv::absdiff(channelsCurrent[0], channelsPrevious[0], luminanceDiff);

    int nonZeroLuminance = cv::countNonZero(luminanceDiff);
    int nonZeroLuminanceThreshold = luminanceDiff.total() * 0.005; // 0.5% of the total pixels in the image

    return nonZeroLuminance < nonZeroLuminanceThreshold;
}

void DuplicateFramesTracker::recordDuplicateFrames(const cv::Mat& current, const cv::Mat& previous, const cv::VideoCapture& cap) {
    const int uniqueFrameThreshold = 10;

    if (previous.empty())
        return;

    // Check if the current frame is a black frame
    cv::Scalar avgPixelIntensity = cv::mean(current);
    if (avgPixelIntensity[0] < 10 && avgPixelIntensity[1] < 10 && avgPixelIntensity[2] < 10) {
		m_start = -1;
		return;
	}

	// Check if the current frame is a solid color frame
	cv::Mat stdDev;
	cv::meanStdDev(current, avgPixelIntensity, stdDev);
	if (stdDev.at<double>(0) < 1 && stdDev.at<double>(1) < 1 && stdDev.at<double>(2) < 1) {
		m_start = -1;
		return;
	}

    // Check if the current frame is a text frame (high contrast)
    cv::Mat gray, contrast;
    cv::cvtColor(current, gray, cv::COLOR_BGR2GRAY);
    cv::Laplacian(gray, contrast, CV_64F); // To catch high contrast edges (usually white text over black background qualifies)
    double contrastLevel = cv::mean(contrast)[0];
    if (contrastLevel > 10.0) {
		m_start = -1;
		return;
	}

    int dupFrame = cap.get(cv::CAP_PROP_POS_FRAMES) - 2;
    if (findDuplicates(current, previous)) {
        if (m_start == -1) {
            m_start = dupFrame;
        }
        uniqueFrameCount = 0;
    } else {
        uniqueFrameCount++;
    }
	if (uniqueFrameCount > uniqueFrameThreshold && m_start != -1) {
		m_end = dupFrame + 1;
		m_duplicateFrames.push_back(std::make_pair(m_start, m_end));
		m_start = -1;
		uniqueFrameCount = 0;
	}
}

void DuplicateFramesTracker::printDuplicateFrames() {
	std::cout << "Duplicate frames: " << m_duplicateFrames.size() << std::endl;
	for (auto &frame : m_duplicateFrames) {
		std::cout << "From " << frame.first << " to " << frame.second << std::endl;
	}
}

void DuplicateFramesTracker::printFreezeFrames() {
	std::cout << "Freeze frames: " << m_freezeFrames.size() << std::endl;
	for (auto &frame : m_freezeFrames) {
		std::cout << "From " << frame.first << " to " << frame.second << std::endl;
	}
}
