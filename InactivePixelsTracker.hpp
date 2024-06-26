#ifndef INACTIVEPIXELSTRACKER_HPP
#define INACTIVEPIXELSTRACKER_HPP

#include <vector>
#include <opencv2/opencv.hpp>
#include <atomic>
#include <thread>
#include <list>
#include "Converter.hpp"
#include "FramesTracker.hpp"

enum class Side { TOP, BOTTOM, LEFT, RIGHT };

class InactivePixelsTracker : public FramesTracker {
public:
	InactivePixelsTracker(double fps) : m_start(-1), m_end(-1) {this->fps = fps; };

	void recordInactivePixels(const cv::Mat &frame, const cv::VideoCapture &cap);
	std::list<std::pair<int, int>> getInactivePixels() { return m_inactivePixels; };
	void appendInactivePixels(std::list<std::pair<int, int>> ipfs);
	void mergeFrameRanges() { mergeRanges(m_inactivePixels, fps); };
	void printInactivePixelsTimecodes() { printTimecodes(m_inactivePixels, fps, "Inactive pixels: "); };
	void printInactivePixelsFrames() { printFrames(m_inactivePixels, fps, "Inactive pixels: "); };

private:
	double fps;
	int m_start;
	int m_end;
	std::list<std::pair<int, int>> m_inactivePixels;

	bool hasInactiveSide(const cv::Mat &gray, Side side, int threshold = 0);
	bool hasInactivePixels(const cv::Mat &frame, int threshold = 3, int innerThreshold = 30);
};

#endif
