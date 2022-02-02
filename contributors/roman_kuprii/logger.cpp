#include <fstream>
#include <iostream>
#include <optional>
#include <string>

#include "logger.hpp"

namespace uni_cpp_practice {

void Logger::set_output(const std::optional<std::string>& file_path) {
  if (!file_path.has_value()) {
    if (file_stream_.has_value()) {
      file_stream_->close();
      file_stream_ = std::nullopt;
    }
    return;
  }

  if (file_stream_.has_value()) {
    file_stream_->close();
  }

  file_stream_ = std::ofstream(file_path.value());

  if (!file_stream_->is_open()) {
    throw std::runtime_error("Failed to create file stream");
  }
}

void Logger::log(const std::string& text) {
  std::cout << text << std::endl;
  if (file_stream_.has_value())
    file_stream_.value() << text << std::endl;
}

Logger::~Logger() {
  if (file_stream_.has_value())
    file_stream_->close();
}

}  // namespace uni_cpp_practice
