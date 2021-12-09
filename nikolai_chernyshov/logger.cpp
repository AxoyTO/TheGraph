#include "logger.hpp"
#include <iostream>
#include "config.hpp"

namespace uni_course_cpp {

void Logger::log(const std::string& string) {
  std::cout << string;
  file_stream_ << string;
}

Logger::~Logger() {
  if (file_stream_.is_open())
    file_stream_.close();
}

Logger::Logger() : file_stream_(std::ofstream(config::log_file_path())) {
  if (!file_stream_.is_open())
    throw std::runtime_error("Can't create file stream");
}
}  // namespace uni_course_cpp
