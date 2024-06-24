#include "Converter.hpp"
#include <iomanip>
#include <sstream>
#include <cmath>

std::string Converter::frameToTimecode(int frame, double fps) {
	int hours = frame / static_cast<int>(fps * 3600);
	int minutes = (frame / static_cast<int>(fps * 60)) % 60;
	int seconds = (frame / static_cast<int>(fps)) % 60;
	int frames = frame % static_cast<int>(fps);
	std::ostringstream timecode;
	timecode << std::setw(2) << std::setfill('0') << hours << ":"
			 << std::setw(2) << std::setfill('0') << minutes << ":"
			 << std::setw(2) << std::setfill('0') << seconds << ":"
			 << std::setw(2) << std::setfill('0') << frames;
	return timecode.str();
}

std::string Converter::frameToDropFrameTimecode(int frame, double fps) {
	int fpsInt = static_cast<int>(fps);
	int dropFrameCount = static_cast<int>(std::round(fps / 30.0)) * 2;

	int totalMinutes = frame / (fpsInt * 60);
	int dropFrames = totalMinutes * dropFrameCount;
	int frames = frame + dropFrames;

	int hours = frames / (fpsInt * 60 * 60);
	frames %= fpsInt * 60 * 60;
	int minutes = frames / (fpsInt * 60);
	frames %= fpsInt * 60;
	int seconds = frames / fpsInt;
	frames %= fpsInt;

	std::ostringstream timecode;
	timecode << std::setw(2) << std::setfill('0') << hours << ":"
			 << std::setw(2) << std::setfill('0') << minutes << ":"
			 << std::setw(2) << std::setfill('0') << seconds << ";"
			 << std::setw(2) << std::setfill('0') << frames;
	return timecode.str();
}

std::string Converter::frameToSubRipTimecode(int frame, double fps) {
	double timeInSeconds = frame / fps;
	int hours = static_cast<int>(timeInSeconds / 3600);
	int minutes = static_cast<int>((timeInSeconds - hours * 3600) / 60);
	int seconds = static_cast<int>(timeInSeconds) % 60;
	int milliseconds = static_cast<int>((timeInSeconds - static_cast<int>(timeInSeconds)) * 1000);
	std::ostringstream timecode;
	timecode << std::setw(2) << std::setfill('0') << hours << ":"
			 << std::setw(2) << std::setfill('0') << minutes << ":"
			 << std::setw(2) << std::setfill('0') << seconds << ","
			 << std::setw(3) << std::setfill('0') << milliseconds;
	return timecode.str();
}
