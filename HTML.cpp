#include "HTML.hpp"

std::string HTML::extractFileNAme(const std::string& path) {
	size_t lastSlash = path.find_last_of('/');
	if (lastSlash == std::string::npos) {
		lastSlash = -1;
	}
	size_t lastDot = path.find_last_of('.');
	if (lastDot == std::string::npos) {
		lastDot = path.size();
	}
	return path.substr(lastSlash + 1, (lastDot - 1) - lastSlash);
}

HTML::HTML(std::string file, std::string videoPath, double fps) {
	m_videoPath = videoPath;
	m_directory = "report_" + extractFileNAme(videoPath) + "/";
	m_file = m_directory + file + ".html";
	m_fps = fps;
};

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
		buttonsHtml += "<button style=\"background-color: #75725B; color: #052327;\" onclick=\"seek("
				+ std::to_string(frame.first) + ")\">From "
				+ frameToSubRipTimecode(frame.first, m_fps)
//				+ " to " + frameToSubRipTimecode(frame.second, m_fps)
				+ " frame: " + std::to_string(frame.first) + "</button>\n";
	}
	return buttonsHtml;
}

int HTML::initJS() {
	std::ifstream templateFileJs("template.js");
	if (!templateFileJs.is_open()) {
		std::cout << "Failed to open template file" << std::endl;
		return 1;
	}

	std::string jsTemplate((std::istreambuf_iterator<char>(templateFileJs)),
						   std::istreambuf_iterator<char>());
	templateFileJs.close();

	replaceAll(jsTemplate, "{{m_fps}}", std::to_string(m_fps));

	std::ofstream jsFile(m_directory + "script.js");
	if (!jsFile.is_open()) {
		std::cout << "Failed to open script.js" << std::endl;
		return 1;
	}

	jsFile << jsTemplate;
	jsFile.close();

	return 0;

}

int HTML::initHTML() {
	std::ifstream templateFileHtml("template_no_js.html");
	if (!templateFileHtml.is_open()) {
		std::cout << "Failed to open template file" << std::endl;
		return 1;
	}

	std::filesystem::path pathDir(m_directory);
	if (m_directory != "./" && !std::filesystem::exists(pathDir)) {
		if (!std::filesystem::create_directory(pathDir)) {
			std::cout << "Failed to create directory " << m_directory << std::endl;
			return 1;
		}
	}

	std::string htmlTemplate((std::istreambuf_iterator<char>(templateFileHtml)),
							 std::istreambuf_iterator<char>());
	templateFileHtml.close();

	replaceAll(htmlTemplate, "{{title}}", extractFileNAme(m_videoPath) + " report");
	replaceAll(htmlTemplate, "{{videoPath}}", m_videoPath);
	replaceAll(htmlTemplate, "{{m_fps}}", std::to_string(m_fps));

	std::ofstream htmlFile(m_file);
	if (!htmlFile.is_open()) {
		std::cout << "Failed to open " << m_file << std::endl;
		return 1;
	}

	htmlFile << htmlTemplate;
	htmlFile.close();

	if (initJS()) {
		std::cout << "Failed to initialize the js file" << std::endl;
		return 1;
	}

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
		htmlContent.insert(pos, "<h3>" + message + std::to_string(frames.size()) + ":</h3>\n" + buttonsHtml);
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
