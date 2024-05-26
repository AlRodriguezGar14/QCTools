#ifndef INACTIVEPIXELSTRACKER_HPP
#define INACTIVEPIXELSTRACKER_HPP

#include <vector>
#include <opencv2/opencv.hpp>

class InactivePixelsTracker {
public:
	InactivePixelsTracker() : m_start(-1), m_end(-1) {};

	void recordInactivePixels(const cv::Mat &frame, const cv::VideoCapture &cap);


private:
	int m_start;
	int m_end;
//	std::list<int> m_inactivePixels;
	std::list<std::pair<int, int>> m_inactivePixels;

	bool hasLetterbox(const cv::Mat &frame, int threshold);
	bool hasPillarbox(const cv::Mat &frame, int threshold);
	bool hasInactivePixels(const cv::Mat &frame, int threshold, int innerThreshold);
};

#endif
