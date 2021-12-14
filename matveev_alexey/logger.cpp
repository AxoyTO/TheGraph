#include "logger.hpp"
#include <chrono>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include "graph.hpp"
#include "graph_printer.hpp"

namespace uni_course_cpp {
void Logger::log(const std::string& string) {
  std::string log_info = getCurrentDateTime();
  log_info += ": " + string;
  std::cout << log_info;
  if (file_stream) {
    *file_stream << log_info;
  }
}

void Logger::setFile(std::optional<std::string> file_path) {
  if (file_stream) {
    file_stream->close();
  }
  if (file_path) {
    file_stream = std::ofstream(file_path.value());
  }
}

Logger& Logger::getLogger() {
  static Logger logger;
  return logger;
}

Logger::~Logger() {
  if (file_stream) {
    (*file_stream).close();
  }
}
std::string Logger::getCurrentDateTime() {
  const auto date_time = std::chrono::system_clock::now();
  const auto date_time_t = std::chrono::system_clock::to_time_t(date_time);
  std::stringstream date_time_string;
  date_time_string << std::put_time(std::localtime(&date_time_t),
                                    "%Y.%m.%d %H:%M:%S");
  return date_time_string.str();
}
}  // namespace uni_course_cpp
