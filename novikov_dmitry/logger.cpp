#include "logger.hpp"
#include <cassert>
#include <iostream>

namespace uni_cpp_practice {

Logger::~Logger() {
  if (file_stream_.has_value()) {
    if (file_stream_->is_open()) {
      file_stream_->close();
    }
  }
}

void Logger::set_file(const std::optional<std::string>& filename) {
  if (!filename.has_value()) {
    if (file_stream_.has_value()) {
      file_stream_->close();
      file_stream_ = std::nullopt;
    }
    return;
  }
  if (file_stream_->is_open()) {
    file_stream_->close();
  }

  file_stream_ = std::ofstream(filename.value());
  if (!file_stream_->is_open()) {
    throw std::runtime_error("Failed to create file stream");
  }
}

void Logger::log(const std::string& string) {
  if (file_stream_.has_value()) {
    file_stream_.value() << string;
  }
  std::cout << string;
}
}  // namespace uni_cpp_practice
