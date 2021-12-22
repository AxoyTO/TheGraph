#include <exception>
#include <fstream>
#include <iostream>

#include "config.hpp"
#include "logger.hpp"

namespace uni_cource_cpp {
Logger::Logger() : output_filestream_(config::log_file_path()) {
  if (!output_filestream_.is_open()) {
    throw std::runtime_error("can't open log file");
  }
}

void Logger::log(const std::string& string) {
  std::cout << string;
  output_filestream_ << string;
}

Logger& Logger::get_logger() {
  static Logger logger;
  return logger;
}
}  // namespace uni_cource_cpp
