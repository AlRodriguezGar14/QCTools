#include <iostream>
#include <string>
#include <opencv2/opencv.hpp>
#include "InactivePixelsTracker.hpp"
#include "duplicateFramesTracker.hpp"

void	printProgressBar(int curr, int total) {
	float	progress = (float)curr / total;
	int 	barWidth = 70;
	int 	position = barWidth * progress;

	std::cout << "[";
	for (int i = 0; i != barWidth; i++) {
		i <= position ? std::cout << "#" : std::cout << " ";
	}
	std::cout << "] " << curr << " / " << total << " | " << int(progress * 100) << "%";
	if (curr != total)
		std::cout << "\r";
	else
		std::cout << "\n";
	std::cout.flush();

}

int main(int argc, char **argv) {

	if (argc != 2) {
		std::cout << "Usage: ./qctool <Video_Path>" << std::endl;
		return 1;
	}

	cv::VideoCapture cap(argv[1]);
	int totalFrames = cap.get(cv::CAP_PROP_FRAME_COUNT);
	int currFrame = 0;

	std::string window_name = argv[1];
	cv::namedWindow(window_name, cv::WINDOW_NORMAL);
	InactivePixelsTracker ipt;
	DuplicateFramesTracker dft;

	int fps = cap.get(cv::CAP_PROP_FPS);
	std::cout << "fps: " << fps << std::endl;
//	cap.set(cv::CAP_PROP_POS_FRAMES, 89600);

	cv::Mat previous;

	while (true) {

		cv::Mat frame;
		bool bSuccess = cap.read(frame);
		if (!bSuccess) {
			std::cout << "End of the video" << std::endl;
			break ;
		}

		// Find duplicate frames or freeze image
		dft.recordDuplicateFrames(frame, previous, cap);
		// Find inactive pixels, letterbox, and pillarbox
//		ipt.recordInactivePixels(frame, cap);

		// Display frame, pending to check if display or not in the input
//		cv::Mat gray;
//		cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);
//		cv::equalizeHist(gray, gray);
//		cv::threshold(gray, gray, 3, 255, cv::THRESH_BINARY);
//		cv::imshow(window_name, gray);
//		char key = cv::waitKey(1);
//		if (key == 27)
//			break ;
//		else if (key == 112)
//			while (true)
//			{
//				if (cv::waitKey(1) == 112)
//					break ;
//			}

		printProgressBar(currFrame++, totalFrames - 1);
		previous = frame;
	}

//	ipt.printInactivePixels();
	dft.printDuplicateFrames();
	dft.printFreezeFrames();
	cv::destroyAllWindows();
	return 0;
}
