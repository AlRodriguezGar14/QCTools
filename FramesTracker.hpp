#ifndef FRAMES_TRACKER_HPP
#define FRAMES_TRACKER_HPP

#include <list>
#include "Converter.hpp"

class FramesTracker : public Converter {
public:
	void	mergeRanges(std::list<std::pair<int, int>> &toMerge, double fps);
	void	printTimecodes(std::list<std::pair<int, int>> frames, double fps, std::string message);
	void	printFrames(std::list<std::pair<int, int>> frames, double fps, std::string message);
};

#endif
