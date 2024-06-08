#include "HTML.hpp"

int HTML::initHTML() {
 std::ofstream htmlFile;
 htmlFile.open(m_file);
 if (!htmlFile.is_open()) {
  std::cout << "Failed to open " << m_file << std::endl;
  return 1;
 }
 htmlFile << "<!DOCTYPE html>\n";
 htmlFile << "<html style=\"height: 100%;\">\n";
 htmlFile << "<head>\n";
 htmlFile << "<title>Analysis Report</title>\n";
 htmlFile << "<style>\n";
 htmlFile << "body, html {height: 100%; margin: 0; padding: 0 2%;background-color: #052327; color: #B59E79;}\n";
 htmlFile << "#video {width: 65%;}\n";
 htmlFile << "</style>\n";
 htmlFile << "</head>\n";
 htmlFile << "<body>\n";
 htmlFile << "<h1>Analysis Report</h1>\n";
 htmlFile << "<video id=\"video\" controls>\n";
 htmlFile << "<source src=\"" << m_videoPath << "\" type=\"video/mp4\">\n";
 htmlFile << "Your browser does not support the video tag.\n";
 htmlFile << "</video>\n";
 htmlFile << "<p id=\"currentFrame\"></p>\n";
 htmlFile << "<p>FPS: " << m_fps << "</p>\n";
 htmlFile.close();
 return 0;
}

int HTML::addButton(std::list<std::pair<int, int>> frames, std::string message) {
    std::ofstream htmlFile;
    if (frames.empty()) {
        return 0;
    }
    htmlFile.open(m_file, std::ios::app);
    if (!htmlFile.is_open()) {
        std::cout << "Failed to open " << m_file << std::endl;
        return 1;
    }
    htmlFile << "<h3>" << message << frames.size() << "</h3>\n";

    for (std::pair<int, int> &frame : frames) {
		htmlFile << "<button style=\"background-color: #75725B; color: #052327;\" onclick=\"seek(" << frame.first << ")\">From " << frameToTimecode(frame.first, m_fps) << " to " << frameToTimecode(frame.second, m_fps)  << "</button>\n";
    }
    htmlFile.close();
    return 0;
}

int HTML::closeHTML() {
 std::ofstream htmlFile;
 htmlFile.open(m_file, std::ios::app);
 if (!htmlFile.is_open()) {
  std::cout << "Failed to open " << m_file << std::endl;
  return 1;
 }
 htmlFile << "<script>\n";
 htmlFile << "var video = document.getElementById('video');\n";
 htmlFile << "var currentFrameElement = document.getElementById('currentFrame');\n";
 htmlFile << "video.addEventListener('timeupdate', function() {\n";
 htmlFile << "    var currentFrame = Math.round(video.currentTime * " << m_fps << ");\n";
 htmlFile << "    currentFrameElement.textContent = 'Current Frame: ' + currentFrame;\n";
 htmlFile << "});\n";
 htmlFile << "var frameTime = 1 / " << m_fps << ";\n";
 htmlFile << "document.addEventListener('keydown', function(event) {\n";
 htmlFile << "    switch (event.key) {\n";
 htmlFile << "        case ',':\n";
 htmlFile << "            video.currentTime = Math.max(video.currentTime - frameTime, 0);\n";
 htmlFile << "            break;\n";
 htmlFile << "        case '.':\n";
 htmlFile << "            video.currentTime = Math.min(video.currentTime + frameTime, video.duration);\n";
 htmlFile << "            break;\n";
 htmlFile << "    }\n";
 htmlFile << "});\n";
 htmlFile << "function seek(frame) {\n";
 htmlFile << "    event.preventDefault();\n";
 htmlFile << "    video.currentTime = (frame + 1) / " << m_fps << ";\n";
 htmlFile << "    video.focus();\n";
 htmlFile << "}\n";
 htmlFile << "</script>\n";

 htmlFile << "</body>\n";
 htmlFile << "</html>\n";
 htmlFile.close();
 return 0;
}
