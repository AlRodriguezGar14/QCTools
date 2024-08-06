#include "duplicateFramesTracker.hpp"
#include <utility>

int	getLuminanceDiff(cv::Mat current, cv::Mat previous) {

	cv::Mat ycrcbCurrent, ycrcbPrevious;
	cv::cvtColor(current, ycrcbCurrent, cv::COLOR_BGR2YCrCb);
	cv::cvtColor(previous, ycrcbPrevious, cv::COLOR_BGR2YCrCb);

	// Split the YCrCb images into their channels. Luminance is the 0th
	std::vector<cv::Mat> channelsCurrent, channelsPrevious;
	cv::split(ycrcbCurrent, channelsCurrent);
	cv::split(ycrcbPrevious, channelsPrevious);

	cv::Mat luminanceDiff;
	cv::absdiff(channelsCurrent[0], channelsPrevious[0], luminanceDiff);

	return cv::countNonZero(luminanceDiff);
}

bool DuplicateFramesTracker::findDuplicates(cv::Mat current, cv::Mat previous) {

    if (previous.empty())
        return false;

	int nonZeroThreshold = current.total() * 0.005; // 0.5% of the total pixels in the image // TODO: Test is it's enough threshold
    cv::Mat diff;
    cv::absdiff(current, previous, diff);
    cv::cvtColor(diff, diff, cv::COLOR_BGR2GRAY);

    int nonZero = cv::countNonZero(diff);

    if (nonZero >= nonZeroThreshold) {
        return false;
    }
    return (getLuminanceDiff(current, previous) < nonZeroThreshold);
}

bool	checkBlackFrame(cv::Scalar avgPixelIntensity) {

	if (avgPixelIntensity[0] < 10 && avgPixelIntensity[1] < 10 && avgPixelIntensity[2] < 10)
		return true;
	return false;
}

bool checkSolidColorFrame(cv::Mat current, cv::Scalar avgPixelIntensity) {

	cv::Mat stdDev;
	cv::meanStdDev(current, avgPixelIntensity, stdDev);
	if (*stdDev.ptr<double>(0) < 1 && *stdDev.ptr<double>(1) < 1 && *stdDev.ptr<double>(2) < 1)
		return true;
	return false;
}

bool checkTextFrame(cv::Mat current) {
	cv::Mat gray, contrast;

	cv::cvtColor(current, gray, cv::COLOR_BGR2GRAY);
	cv::Laplacian(gray, contrast, CV_64F); // To catch high contrast edges (usually white text over black background qualifies) // TODO: Test it more
	double contrastLevel = cv::mean(contrast)[0];
	if (contrastLevel > 10.0)
		return true;
	return false;
}

void DuplicateFramesTracker::recordDuplicateFrames(const cv::Mat& current, const cv::Mat& previous, const cv::VideoCapture& cap) {

    if (previous.empty()) return;

	cv::Scalar avgPixelIntensity = cv::mean(current);
	const int uniqueFrameThreshold = round(m_fps / 2);
	const int uniqueFrameCountThreshold = 2;

	if (checkBlackFrame(avgPixelIntensity)) {
		m_start = -1;
		return;
	}

	if (checkSolidColorFrame(current, avgPixelIntensity)) {
		m_start = -1;
		return;
	}

	if (checkTextFrame(current)) {
		m_start = -1;
		return;
	}

    int dupFrame = cap.get(cv::CAP_PROP_POS_FRAMES) - uniqueFrameCountThreshold;
    if (findDuplicates(current, previous)) {
        if (m_start == -1) {
            m_start = dupFrame;
        }
		m_uniqueFrameCount = 0;
    } else {
        m_uniqueFrameCount++;
    }
	if (m_uniqueFrameCount > uniqueFrameThreshold && m_start != -1) {
		m_end = dupFrame;
		m_duplicateFrames.push_back(std::make_pair(m_start, m_end));
		m_start = -1;
		m_uniqueFrameCount = 0;
	}
}

void	DuplicateFramesTracker::appendDuplicateFrames(std::list<std::pair<int, int>> dpfs) {
	this->m_duplicateFrames.insert(this->m_duplicateFrames.end(), dpfs.begin(), dpfs.end());
}
