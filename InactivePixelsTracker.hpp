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
	InactivePixelsTracker(double fps) : FramesTracker(fps) {};

	void recordInactivePixels(const cv::Mat &frame, const cv::VideoCapture &cap);
	std::list<std::pair<int, int>> getInactivePixels() { return m_inactivePixels; };
	void appendInactivePixels(std::list<std::pair<int, int>> ipfs);
	void mergeFrameRanges() { mergeRanges(m_inactivePixels, m_fps); };

private:
	std::list<std::pair<int, int>> m_inactivePixels;
	bool hasInactiveSide(const cv::Mat &gray, Side side, int threshold);
	bool hasInactivePixels(const cv::Mat &frame, int threshold, int innerThreshold);
};

#endif
