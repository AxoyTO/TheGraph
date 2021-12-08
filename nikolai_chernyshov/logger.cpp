#include "logger.hpp"
#include <iostream>

namespace uni_course_cpp {

void Logger::set_file(const std::optional<std::string>& filename) {
  if (!filename.has_value()) {
    if (file_stream_.has_value()) {
      file_stream_->close();
      file_stream_ = std::nullopt;
    }
    return;
  }

  if (file_stream_->is_open())
    file_stream_->close();

  file_stream_ = std::ofstream(filename.value());
  if (!file_stream_->is_open())
    throw std::runtime_error("Can't create file stream");
}

void Logger::log(const std::string& string) {
  std::cout << string;
  if (file_stream_.has_value())
    file_stream_.value() << string;
}

Logger::~Logger() {
  if (file_stream_.has_value())
    if (file_stream_->is_open())
      file_stream_->close();
}
}  // namespace uni_course_cpp
