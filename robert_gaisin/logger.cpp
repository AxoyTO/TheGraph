#include "logger.hpp"

#include <iostream>

namespace uni_cource_cpp {

void Logger::log(const std::string& log_string) {
  std::lock_guard lock(log_mutex);
  std::cout << log_string << std::endl;
  file_stream_ << log_string << std::endl;
}

Logger::~Logger() {
  file_stream_.close();
}
}  // namespace uni_cource_cpp
