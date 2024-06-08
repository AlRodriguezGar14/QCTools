#ifndef PROGRESS_TRACKER_HPP
#define PROGRESS_TRACKER_HPP

#include <vector>
#include <atomic>
#include <memory>
#include <iostream>
#include <thread>
#include <chrono>
#include <cmath>

class ProgressTracker {

public:
    ProgressTracker(int numberOfSegments);
    void updateProgress(int current, int total, int threadId);
    void printProgress();
    void stop();

private:
    std::vector<std::shared_ptr<std::atomic<int>>> progress;
    std::atomic<bool> threadsRunning;
};

#endif
