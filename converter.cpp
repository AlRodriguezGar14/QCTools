#include "Converter.hpp"
#include <iomanip>
#include <sstream>

std::string Converter::frameToTimecode(int frame, double fps) {
	int fpsInt = static_cast<int>(fps);
	int hours = frame / (fpsInt * 60 * 60);
	int minutes = (frame / (fpsInt * 60)) % 60;
	int seconds = (frame / fpsInt) % 60;
	int frames = frame % fpsInt;
	std::ostringstream timecode;
	timecode << std::setw(2) << std::setfill('0') << hours << ":"
			 << std::setw(2) << std::setfill('0') << minutes << ":"
			 << std::setw(2) << std::setfill('0') << seconds << ":"
			 << std::setw(2) << std::setfill('0') << frames;
	return timecode.str();
}

// if fps == 30 * 1000 / 1001
std::string Converter::frameToDropFrameTimecode(int frame, double fps) {
	int fpsInt = static_cast<int>(fps);
	int hours = frame / (fpsInt * 60 * 60);
	int minutes = (frame / (fpsInt * 60)) % 60;
	int seconds = (frame / fpsInt) % 60;
	int dropFrames = static_cast<int>(frame * 0.001) - static_cast<int>(fpsInt * 60 * 60 * hours) - static_cast<int>(fpsInt * 60 * minutes) - static_cast<int>(fpsInt * seconds);
	std::ostringstream timecode;
	timecode << std::setw(2) << std::setfill('0') << hours << ":"
			 << std::setw(2) << std::setfill('0') << minutes << ":"
			 << std::setw(2) << std::setfill('0') << seconds << ";"
			 << std::setw(2) << std::setfill('0') << dropFrames;
	return timecode.str();
}

std::string Converter::frameToSubRipTimecode(int frame, double fps) {
	int fpsInt = static_cast<int>(fps);
	int hours = frame / (fpsInt * 60 * 60);
	int minutes = (frame / (fpsInt * 60)) % 60;
	int seconds = (frame / fpsInt) % 60;
	int frames = frame % fpsInt;
	std::ostringstream timecode;
	timecode << std::setw(2) << std::setfill('0') << hours << ":"
			 << std::setw(2) << std::setfill('0') << minutes << ":"
			 << std::setw(2) << std::setfill('0') << seconds << ","
			 << std::setw(2) << std::setfill('0') << frames;
	return timecode.str();
}