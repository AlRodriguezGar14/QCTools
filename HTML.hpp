#ifndef HTML_HPP
#define HTML_HPP

#include <string>
#include <list>
#include <fstream>
#include <iostream>
#include "Converter.hpp"

class HTML : public Converter {
public:
	HTML(std::string file, std::string videoPath, double fps) : m_file(file + ".html"), m_videoPath(videoPath), m_fps(fps) {};
	int initHTML();
	int addButton(std::list<std::pair<int, int>> frames, std::string message);
	int closeHTML();

private:
	std::string m_file;
	std::string m_videoPath;
	double m_fps;
};

#endif

