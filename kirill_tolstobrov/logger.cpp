#include "logger.hpp"

#include <fstream>
#include <iostream>

namespace uni_cpp_practice {
void Logger::set_file(const std::optional<std::string>& filename) {
  if (!filename.has_value()) {
    file_stream_ = std::nullopt;
    return;
  }

  file_stream_ = std::ofstream(filename.value());
  if (!file_stream_->is_open()) {
    throw std::runtime_error("Can't open log file");
  }
}

void Logger::log(const std::string& string) {
  if (file_stream_.has_value()) {
    file_stream_.value() << string;
  }
  std::cout << string;
}

Logger::~Logger() {
  if (file_stream_.has_value() && file_stream_->is_open()) {
    file_stream_->close();
    file_stream_ = std::nullopt;
  }
}
}  // namespace uni_cpp_practice
