#ifndef CONVERTER_HPP
#define CONVERTER_HPP

#include <string>

class Converter {
protected:
	std::string frameToTimecode(int frame, double fps);
	std::string frameToDropFrameTimecode(int frame, double fps);
	std::string frameToSubRipTimecode(int frame, double fps);
};

#endif
