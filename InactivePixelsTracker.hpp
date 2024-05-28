#ifndef INACTIVEPIXELSTRACKER_HPP
#define INACTIVEPIXELSTRACKER_HPP

#include <vector>
#include <opencv2/opencv.hpp>
#include <atomic>
#include <thread>

enum class Side { TOP, BOTTOM, LEFT, RIGHT };

class InactivePixelsTracker {
public:
	InactivePixelsTracker() : m_start(-1), m_end(-1) {};

	void recordInactivePixels(const cv::Mat &frame, const cv::VideoCapture &cap);
	void printInactivePixels();

private:
	int m_start;
	int m_end;
	std::list<std::pair<int, int>> m_inactivePixels;

	bool hasInactiveSide(const cv::Mat &gray, Side side, int threshold = 0);
	bool hasInactivePixels(const cv::Mat &frame, int threshold = 3, int innerThreshold = 30);
};

#endif