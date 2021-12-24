#include "logger.hpp"
#include <iostream>

namespace uni_cpp_practice {
void Logger::set_file(const std::optional<std::string>& filename) {
  if (!filename.has_value()) {
    if (file_stream_.has_value()) {
      file_stream_->close();
      file_stream_ = std::nullopt;
    }
    return;
  }

  if (file_stream_.has_value()) {
    file_stream_->close();
  }

  file_stream_ = std::ofstream(filename.value());
  if (!file_stream_->is_open())
    throw std::runtime_error("Error while opening the log file!");
}

void Logger::log(const std::string& string) {
  const std::lock_guard lock(mutex_);
  if (file_stream_.has_value())
    file_stream_.value() << string;
  std::cout << string;
}

Logger::~Logger() {
  if (file_stream_.has_value())
    if (file_stream_->is_open()) {
      file_stream_->close();
      file_stream_ = std::nullopt;
    }
}

}  // namespace uni_cpp_practice
