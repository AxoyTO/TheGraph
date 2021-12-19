#include "logger.hpp"
#include <fstream>
#include <iostream>

namespace uni_cource_cpp {

void Logger::log(const std::string& string) {
  file_stream_ << string << std::endl;
  std::cout << string << std::endl;
}

Logger::Logger() : file_stream_(config::logger_file_path()) {
  if (!file_stream_.is_open()) {
    throw std::runtime_error("...");
  }
  file_stream_.clear();
}

}  // namespace uni_cource_cpp
