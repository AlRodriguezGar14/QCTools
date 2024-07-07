#include <opencv2/opencv.hpp>
#include <future>
#include <fstream>
#include <vector>
#include <thread>
#include <memory>
#include "InactivePixelsTracker.hpp"
#include "duplicateFramesTracker.hpp"
#include "Converter.hpp"
#include "HTML.hpp"
#include "ProgressTracker.hpp"

typedef std::tuple<InactivePixelsTracker, DuplicateFramesTracker> InactivePixelsAndDuplicateFrames;
typedef std::future<std::tuple<InactivePixelsTracker, DuplicateFramesTracker>> FutureInactivePixelsAndDuplicateFrames;

InactivePixelsAndDuplicateFrames processSegment(std::string videoPath, int startFrame, int endFrame, double fps, int threadId, ProgressTracker *progressTracker) {
    cv::VideoCapture cap(videoPath);
    cap.set(cv::CAP_PROP_POS_FRAMES, startFrame);

    InactivePixelsTracker ipt(fps);
    DuplicateFramesTracker dft(fps);

    cv::Mat previous;
    for (int i = startFrame; i < endFrame; ++i) {
        cv::Mat frame;
        bool success = cap.read(frame);

        if (!success) {
            std::cout << "Failed to read frame " << i << std::endl;
            break;
        }

        dft.recordDuplicateFrames(frame, previous, cap);
        ipt.recordInactivePixels(frame, cap);
        previous = frame.clone();
		progressTracker->updateProgress(i - startFrame, endFrame - startFrame, threadId);
    }

    return std::make_tuple(ipt, dft);
}

int main(int argc, char **argv) {
    if (argc != 2) {
        std::cout << "Usage: ./qctool <Video_Path>" << std::endl;
        return 1;
    }

    std::string videoPath = argv[1];
    cv::VideoCapture cap(videoPath);
	if (!cap.isOpened()) return 1;
    double totalFrames = cap.get(cv::CAP_PROP_FRAME_COUNT);
    double fps = cap.get(cv::CAP_PROP_FPS);

    std::cout << "fps: " << fps << std::endl;
    std::cout << "title: " << argv[1] << std::endl;
    std::cout << "Processing..." << std::endl;

    DuplicateFramesTracker dft(fps);
    InactivePixelsTracker ipt(fps);
    HTML report("report", videoPath, fps);
    int numberOfSegments = 4; // Update this to the number of threads to run
	ProgressTracker progressTracker(numberOfSegments);

    std::thread progressThread(&ProgressTracker::printProgress, &progressTracker);

    std::vector<FutureInactivePixelsAndDuplicateFrames> futures;
    for (int i = 0; i < numberOfSegments; ++i) {
        int startFrame = i * totalFrames / numberOfSegments;
        int endFrame = (i + 1) * totalFrames / numberOfSegments;
        futures.push_back(std::async(std::launch::async, processSegment, videoPath, startFrame, endFrame, fps, i, &progressTracker));
    }

    for (FutureInactivePixelsAndDuplicateFrames &fut : futures) {
        InactivePixelsAndDuplicateFrames res = fut.get();
        ipt.appendInactivePixels(std::get<0>(res).getInactivePixels());
        dft.appendDuplicateFrames(std::get<1>(res).getDuplicateFrames());
    }

	progressTracker.stop();
    progressThread.join();

    ipt.mergeFrameRanges();
    dft.mergeFrameRanges();
    if (report.initHTML())
		return 1;
    if (report.addButton(ipt.getInactivePixels(), "Inactive Pixels: ", "ipf"))
		return 1;
    if (report.addButton(dft.getDuplicateFrames(), "Duplicate Frames: ", "dpf"))
		return 1;

	std::cout << "\nReport generated" << std::endl;

    return 0;
}
