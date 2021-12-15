#include "logger.hpp"
#include <iostream>
#include "config.hpp"

namespace uni_cource_cpp {
void Logger::log(const std::string& string) {
  file_stream_ << string;
  std::cout << string;
}

Logger::Logger() : file_stream_(config::log_file_path()) {
  if (!file_stream_.is_open())
    throw std::runtime_error("Failed to create file stream");
}

}  // namespace uni_cource_cpp
