#ifndef HTML_HPP
#define HTML_HPP

#include <string>
#include <filesystem>
#include <list>
#include <fstream>
#include <iostream>
#include "Converter.hpp"

class HTML : public Converter {
public:
	HTML(std::string file, std::string videoPath, double fps);
	int initHTML();
	int addButton(const std::list<std::pair<int, int>>& frames, const std::string& message, const std::string& buttonClass);
	std::string extractFileNAme(const std::string& path);

private:
	std::string m_videoPath;
	std::string m_directory;
	std::string m_file;
	double m_fps;

	int initJS();
	void replaceAll(std::string& str, const std::string& from, const std::string& to);
	std::string generateButtonsHtml(const std::list<std::pair<int, int>>& frames, const std::string& buttonClass);
};

#endif

