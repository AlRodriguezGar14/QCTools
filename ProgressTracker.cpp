#include "ProgressTracker.hpp"

ProgressTracker::ProgressTracker(int numberOfSegments) : threadsRunning(true) {
	progress.resize(numberOfSegments);
	for(std::shared_ptr<std::atomic<int>>& p : progress) {
		p = std::make_shared<std::atomic<int>>(0);
	}
}

void ProgressTracker::updateProgress(int current, int total, int threadId) {
	*progress[threadId] = round(current * 100.0) / total;
}

void ProgressTracker::printProgress() {
    int direction = 1;
    int position = 0;

    while (threadsRunning) {
        std::cout << "\r";
        for (size_t i = 0; i < progress.size(); ++i) {
            std::cout << "Thread " << i << ": [";
            int progressPercentage = *progress[i];
            int barLength = progressPercentage / 10;

            for (int j = 0; j < 10; ++j) {
                if (j == position) {
                    std::cout << "o";
                    if (j < barLength && position != 0 && position != 9) {
                        direction *= -1;
                    }
                } else if (j < barLength) {
                    std::cout << "#";
                } else {
                    std::cout << " ";
                }
            }

            std::cout << "] " << progressPercentage << "%";
            if (i != progress.size() - 1) {
                std::cout << "\t";
            }
        }
        std::cout << std::flush;
        std::this_thread::sleep_for(std::chrono::milliseconds(160));

        position += direction;
        if (position == 9 || position == 0) {
            direction *= -1;
        }
    }
}
void ProgressTracker::stop() {
	threadsRunning = false;
}
