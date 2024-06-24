#include "HTML.hpp"

void HTML::replaceAll(std::string& str, const std::string& from, const std::string& to) {
	size_t start_pos = 0;
	while ((start_pos = str.find(from, start_pos)) != std::string::npos) {
		str.replace(start_pos, from.length(), to);
		start_pos += to.length();
	}
}

std::string HTML::generateButtonsHtml(const std::list<std::pair<int, int>>& frames) {
	std::string buttonsHtml;
	for (const std::pair<int, int>& frame : frames) {
		buttonsHtml += "<button style=\"background-color: #75725B; color: #052327;\" onclick=\"seek(" + std::to_string(frame.first) + ")\">From "
					   + frameToTimecode(frame.first, m_fps) + " to " + frameToTimecode(frame.second, m_fps) + "</button>\n";
	}
	return buttonsHtml;
}

int HTML::initHTML() {
	std::ifstream templateFile("template.html");
	if (!templateFile.is_open()) {
		std::cout << "Failed to open template file" << std::endl;
		return 1;
	}

	std::string htmlTemplate((std::istreambuf_iterator<char>(templateFile)),
							 std::istreambuf_iterator<char>());
	templateFile.close();

	replaceAll(htmlTemplate, "{{title}}", "Analysis Report");
	replaceAll(htmlTemplate, "{{videoPath}}", m_videoPath);
	replaceAll(htmlTemplate, "{{fps}}", std::to_string(m_fps));

	std::ofstream htmlFile(m_file);
	if (!htmlFile.is_open()) {
		std::cout << "Failed to open " << m_file << std::endl;
		return 1;
	}

	htmlFile << htmlTemplate;
	htmlFile.close();

	return 0;
}

int HTML::addButton(const std::list<std::pair<int, int>>& frames, const std::string& message) {
	std::ifstream htmlFileIn(m_file);
	if (!htmlFileIn.is_open()) {
		std::cout << "Failed to open " << m_file << std::endl;
		return 1;
	}

	std::string htmlContent((std::istreambuf_iterator<char>(htmlFileIn)),
							std::istreambuf_iterator<char>());
	htmlFileIn.close();

	std::string buttonsHtml = generateButtonsHtml(frames);

	size_t pos = htmlContent.find("<!--{{buttons}}-->");
	if (pos != std::string::npos) {
		htmlContent.append("<h3>" + message + std::to_string(frames.size()) + "</h3>\n" + buttonsHtml);
	}

	std::ofstream htmlFileOut(m_file);
	if (!htmlFileOut.is_open()) {
		std::cout << "Failed to open " << m_file << std::endl;
		return 1;
	}

	htmlFileOut << htmlContent;
	htmlFileOut.close();

	return 0;
}
