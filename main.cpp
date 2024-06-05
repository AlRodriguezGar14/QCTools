#include <opencv2/opencv.hpp>
#include <future>
#include "InactivePixelsTracker.hpp"
#include "duplicateFramesTracker.hpp"

std::tuple<InactivePixelsTracker, DuplicateFramesTracker> processSegment(std::string videoPath, int startFrame, int endFrame, double fps) {
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

		previous = frame;
	}

	// ipt.printInactivePixels();
	// dft.printDuplicateFrames();
	return std::make_tuple(ipt, dft);
}

int main(int argc, char **argv) {
	if (argc != 2) {
		std::cout << "Usage: ./qctool <Video_Path>" << std::endl;
		return 1;
	}

	std::string videoPath = argv[1];
	cv::VideoCapture cap(videoPath);
	int totalFrames = cap.get(cv::CAP_PROP_FRAME_COUNT);
	double fps = cap.get(cv::CAP_PROP_FPS);
	std::cout << "fps: " << fps << std::endl;

	std::future<std::tuple<InactivePixelsTracker, DuplicateFramesTracker>> fut1 = std::async(std::launch::async, processSegment, videoPath, 0, totalFrames/4, fps);
    std::future<std::tuple<InactivePixelsTracker, DuplicateFramesTracker>> fut2 = std::async(std::launch::async, processSegment, videoPath, totalFrames/4, totalFrames/2, fps);
    std::future<std::tuple<InactivePixelsTracker, DuplicateFramesTracker>> fut3 = std::async(std::launch::async, processSegment, videoPath, totalFrames/2, totalFrames*3/4, fps);
    std::future<std::tuple<InactivePixelsTracker, DuplicateFramesTracker>> fut4 = std::async(std::launch::async, processSegment, videoPath, totalFrames*3/4, totalFrames, fps);

    std::tuple<InactivePixelsTracker, DuplicateFramesTracker> res1 = fut1.get();
    std::tuple<InactivePixelsTracker, DuplicateFramesTracker> res2 = fut2.get();
    std::tuple<InactivePixelsTracker, DuplicateFramesTracker> res3 = fut3.get();
    std::tuple<InactivePixelsTracker, DuplicateFramesTracker> res4 = fut4.get();


	std::get<1>(res1).printDuplicateFrames();
	std::get<1>(res2).printDuplicateFrames();
	std::get<1>(res3).printDuplicateFrames();
	std::get<1>(res4).printDuplicateFrames();

	return 0;
}