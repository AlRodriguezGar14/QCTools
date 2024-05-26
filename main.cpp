#include <iostream>
#include <string>
#include <opencv2/opencv.hpp>
#include "InactivePixelsTracker.hpp"

int main(int argc, char **argv) {

	if (argc != 2) {
		std::cout << "Usage: ./qctool <Video_Path>" << std::endl;
		return 1;
	}

	cv::VideoCapture cap(argv[1]);
	std::string window_name = argv[1];
	cv::namedWindow(window_name, cv::WINDOW_NORMAL);
	InactivePixelsTracker ipt;

	int fps = cap.get(cv::CAP_PROP_FPS);
	std::cout << "fps: " << fps << std::endl;

	while (true) {

		cv::Mat frame;
		bool bSuccess = cap.read(frame);
		if (!bSuccess) {
			std::cout << "End of the video" << std::endl;
			break ;
		}

		// Find inactive pixels, letterbox, and pillarbox
		ipt.recordInactivePixels(frame, cap);

		// Display frame, pending to check if display or not in the input
//		cv::imshow(window_name, frame);
//		char key = cv::waitKey(1);
//		if (key == 27)
//			break ;
//		else if (key == 112)
//			while (true)
//			{
//				if (cv::waitKey(1) == 112)
//					break ;
//			}

	}

	cv::destroyAllWindows();
	return 0;
}
