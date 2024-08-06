#ifndef FRAMES_TRACKER_HPP
#define FRAMES_TRACKER_HPP

#include <list>
#include "Converter.hpp"

class FramesTracker : public Converter {
protected:
	double m_fps;
	int m_start;
	int m_end;
public:
	FramesTracker(double fps) : m_fps(fps), m_start(-1), m_end(-1) {};
	void	mergeRanges(std::list<std::pair<int, int>> &toMerge, double fps);
	void	printTimecodes(std::list<std::pair<int, int>> frames, double fps, std::string message);
	void	printFrames(std::list<std::pair<int, int>> frames, double fps, std::string message);
};

#endif
