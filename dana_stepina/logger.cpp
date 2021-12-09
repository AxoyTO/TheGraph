#include "logger.hpp"

#include <fstream>
#include <iostream>
#include <optional>
#include <string>

namespace uni_cource_cpp {
void Logger::log(const std::string& log_string) {
  std::cout << log_string << std::endl;

  if (file_stream_.has_value())
    file_stream_.value() << log_string << std::endl;
}

void Logger::set_file(const std::optional<std::string>& filename) {
  if (!filename.has_value()) {
    if (file_stream_.has_value()) {
      file_stream_->close();
      file_stream_ = std::nullopt;
    }
    return;
  }

  file_stream_ = std::ofstream(filename.value());

  if (!file_stream_->is_open()) {
    throw std::runtime_error("Failed to create file stream");
  }
}

Logger::~Logger() {
  if (file_stream_.has_value())
    file_stream_->close();
}
}  // namespace uni_cource_cpp
