#ifndef DUPLICATEFRAMESTRACKER_HPP
#define DUPLICATEFRAMESTRACKER_HPP

#include <list>
#include <opencv2/opencv.hpp>

class DuplicateFramesTracker {
public:
	DuplicateFramesTracker(double fps) : m_start(-1), m_end(-1), uniqueFrameCount(0) {this->fps = fps; };

	void recordDuplicateFrames(const cv::Mat& current, const cv::Mat& previous, const cv::VideoCapture& cap);
	void printDuplicateFrames();
	void printFreezeFrames();
	void mergeDuplicateFramesRanges();

private:
	double fps;
	int m_start;
	int m_end;
	int uniqueFrameCount;
	std::list<std::pair<int, int>> m_duplicateFrames;
	std::list<std::pair<int, int>> m_freezeFrames;
	bool findDuplicates(cv::Mat current, cv::Mat previous);

};

#endif
