#include "HTML.hpp"

std::string HTML::extractFileName(const std::string& path) {
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
    m_directory = "report_" + extractFileName(videoPath) + "/";
	m_html_in = "template_no_js.html";
    m_html_out = m_directory + file + ".html";
    m_fps = fps;
	m_js_in = "template.js";
	m_js_out = "script.js";
}

void HTML::replaceAll(std::string& str, const std::string& from, const std::string& to) {
    size_t start_pos = 0;
    while ((start_pos = str.find(from, start_pos)) != std::string::npos) {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length();
    }
}

std::string HTML::generateButtonsHtml(const std::list<std::pair<int, int>>& frames, const std::string& buttonClass) {
    std::string buttonsHtml;
    for (const std::pair<int, int>& frame : frames) {
        buttonsHtml += "<button class=\""
                + buttonClass
                + "\" style=\"background-color: #75725B; color: #052327;\" onclick=\"seek("
                + std::to_string(frame.first) + ")\">From "
                + frameToSubRipTimecode(frame.first, m_fps)
                + " frame: " + std::to_string(frame.first) + "</button>\n";
    }
    return buttonsHtml;
}

bool HTML::readFile(const std::string& filePath, std::string& content) {
    std::ifstream fileIn(filePath);
    if (!fileIn.is_open()) {
        std::cout << "Failed to open " << filePath << std::endl;
        return false;
    }
    content.assign((std::istreambuf_iterator<char>(fileIn)), std::istreambuf_iterator<char>());
    fileIn.close();
    return true;
}

bool HTML::writeFile(const std::string& filePath, const std::string& content) {
    std::ofstream fileOut(filePath);
    if (!fileOut.is_open()) {
        std::cout << "Failed to open " << filePath << std::endl;
        return false;
    }
    fileOut << content;
    fileOut.close();
    return true;
}

int HTML::initJS() {
    std::string jsTemplate;
    if (!readFile(m_js_in, jsTemplate)) {
        return 1;
    }

    replaceAll(jsTemplate, "{{m_fps}}", std::to_string(m_fps));

    if (!writeFile(m_directory + m_js_out, jsTemplate)) {
        return 1;
    }

    return 0;
}

int HTML::initHTML() {
    std::string htmlTemplate;
    if (!readFile(m_html_in, htmlTemplate)) {
        return 1;
    }

    std::filesystem::path pathDir(m_directory);
    if (m_directory != "./" && !std::filesystem::exists(pathDir)) {
        if (!std::filesystem::create_directory(pathDir)) {
            std::cout << "Failed to create directory " << m_directory << std::endl;
            return 1;
        }
    }

    replaceAll(htmlTemplate, "{{title}}", extractFileName(m_videoPath) + " report");
    replaceAll(htmlTemplate, "{{videoPath}}", m_videoPath);
    replaceAll(htmlTemplate, "{{m_fps}}", std::to_string(m_fps));

    if (!writeFile(m_html_out, htmlTemplate)) {
        return 1;
    }

    if (initJS()) {
        std::cout << "Failed to initialize the js file" << std::endl;
        return 1;
    }

    return 0;
}

int HTML::addButton(const std::list<std::pair<int, int>>& frames, const std::string& message, const std::string& buttonClass) {
    std::string htmlContent;
    if (!readFile(m_html_out, htmlContent)) {
        return 1;
    }

    std::string buttonsHtml = generateButtonsHtml(frames, buttonClass);

    size_t pos = htmlContent.find("<!--{{buttons}}-->");
    if (pos != std::string::npos) {
        htmlContent.insert(pos, "<h3>" + message + std::to_string(frames.size()) + ":</h3>\n" + buttonsHtml);
    }

    if (!writeFile(m_html_out, htmlContent)) {
        return 1;
    }

    return 0;
}

int HTML::updateHTMLContent(const std::string& toReplace, const std::string& replacement) {
    std::string htmlContent;
    if (!readFile(m_html_out, htmlContent)) {
        return 1;
    }

    replaceAll(htmlContent, toReplace, replacement);

    if (!writeFile(m_html_out, htmlContent)) {
        return 1;
    }

    return 0;
}

int HTML::generateWaveform(const std::string& imageName) {
    std::string outputImagePath = m_directory + imageName;
    std::string command = "ffmpeg -i " + m_videoPath + " -filter_complex "
                          "\"[0:a]aformat=channel_layouts=mono, "
                          "showwavespic=s=1920x240:colors=white\" "
                          "-frames:v 1 " + outputImagePath;
    int result = std::system(command.c_str());
    if (result != 0) {
        std::cerr << "Error generating waveform: FFmpeg command failed." << std::endl;
        return 1;
    }
    std::cout << "Waveform generated successfully." << std::endl;
    updateHTMLContent("{{waveformImagePath}}", imageName);
    return 0;
}

void HTML::openReport() {
	std::string command = "open " + m_html_out;
	std::system(command.c_str());
}