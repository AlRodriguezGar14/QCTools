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
	std::string extractFileName(const std::string& path);
	int generateWaveform(const std::string& imageName);
	int updateHTMLContent(const std::string& toReplace, const std::string& replacment);
	void openReport();

private:
	std::string m_videoPath;
	std::string m_directory;
	std::string m_html_out;
	std::string m_html_in;
	std::string m_js_in;
	std::string m_js_out;
	double m_fps;

	int initJS();
	void replaceAll(std::string& str, const std::string& from, const std::string& to);
	std::string generateButtonsHtml(const std::list<std::pair<int, int>>& frames, const std::string& buttonClass);
	bool readFile(const std::string& file, std::string& content);
	bool writeFile(const std::string& file, const std::string& content);
};

#endif

