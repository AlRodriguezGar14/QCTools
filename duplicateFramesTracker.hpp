#ifndef DUPLICATEFRAMESTRACKER_HPP
#define DUPLICATEFRAMESTRACKER_HPP

#include <list>
#include <opencv2/opencv.hpp>
#include "Converter.hpp"
#include "FramesTracker.hpp"

class DuplicateFramesTracker : public FramesTracker {
public:
	DuplicateFramesTracker(double fps) : fps(fps), m_start(-1), m_end(-1), uniqueFrameCount(0) {};

	void recordDuplicateFrames(const cv::Mat& current, const cv::Mat& previous, const cv::VideoCapture& cap);
	void printFreezeFrames();
	void mergeFrameRanges() { mergeRanges(m_duplicateFrames, fps); };
	std::list<std::pair<int, int>> getDuplicateFrames() { return m_duplicateFrames; };
	void appendDuplicateFrames(std::list<std::pair<int, int>> dpfs);
	void printDuplicateFramesTimecodes() { printTimecodes(m_duplicateFrames, fps, "Duplicate frames: "); };
	void printDuplicateFrames() { printFrames(m_duplicateFrames, fps, "Duplicate frames: "); };

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
