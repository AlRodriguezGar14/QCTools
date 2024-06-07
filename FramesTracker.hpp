#ifndef FRAMES_TRACKER_HPP
#define FRAMES_TRACKER_HPP

#include <list>

class FramesTracker {
public:
	void	mergeFrames(std::list<std::pair<int, int>> &toMerge, double fps);

protected:
	std::list<std::pair<int, int>> m_frames;

};

#endif
