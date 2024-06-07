#include "FramesTracker.hpp"
#include <iostream>

void FramesTracker::mergeFrames(std::list<std::pair<int, int>> &toMerge, double fps) {
	std::list<std::pair<int, int>> mergedFrames;
	std::list<std::pair<int, int>>::iterator it = toMerge.begin();

	int start = it->first;
	int end = it->second;
	++it;
	for (; it != toMerge.end(); ++it) {
		if (it->first - end < floor(fps)) {
			end = it->second;
		} else {
			mergedFrames.push_back(std::make_pair(start, end));
			start = it->first;
			end = it->second;
		}
	}
	if (end > 1)
		mergedFrames.push_back(std::make_pair(start, end));
	toMerge = mergedFrames;
}

