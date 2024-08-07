#include "FramesTracker.hpp"
#include <iostream>

void FramesTracker::mergeRanges(std::list<std::pair<int, int>> &toMerge, double fps) {
    if (toMerge.empty()) {
        return;
    }
	toMerge.sort();
    std::list<std::pair<int, int>> mergedFrames;
    auto it = toMerge.begin();

    int start = it->first;
    int end = it->second;
    ++it;

    for (; it != toMerge.end(); ++it) {
        if (it->first - end <static_cast<int>(floor(fps))) {
            end = it->second;
        } else {
            mergedFrames.push_back(std::make_pair(start, end));
            start = it->first;
            end = it->second;
        }
    }

    mergedFrames.push_back(std::make_pair(start, end));
    toMerge = mergedFrames;
}

void FramesTracker::printTimecodes(std::list<std::pair<int, int>> frames, double fps, std::string message) {

	if (frames.empty())
		return;
	mergeRanges(frames, fps);
	std::cout << message << frames.size() << std::endl;
	for (std::pair<int, int> &frame : frames) {
		std::cout << "From " << frameToTimecode(frame.first, fps) << " to " << frameToTimecode(frame.second, fps) << std::endl;
	}
}

void FramesTracker::printFrames(std::list<std::pair<int, int>> frames, double fps, std::string message) {

	if (frames.empty())
		return;
	mergeRanges(frames, fps);
	std::cout << message << frames.size() << std::endl;
	for (std::pair<int, int> &frame : frames) {
		std::cout << "From " << frame.first << " to " << frame.second << std::endl;
	}
}
