#include "logger.hpp"
#include <assert.h>
#include <iostream>

namespace uni_cpp_practice {

void Logger::set_file(const std::optional<std::string>& filename) {
  if (!filename.has_value()) {
    file_stream_->close();
    file_stream_ = std::nullopt;
    return;
  }
  if (file_stream_->is_open()) {
    file_stream_->close();
  }

  file_stream_ = std::ofstream(filename.value());
  assert(file_stream_->is_open() && "Error while opening the log file!");
}

void Logger::log(const std::string& string) {
  if (file_stream_.has_value()) {
    file_stream_.value() << string;
  }
  std::cout << string;
}
}  // namespace uni_cpp_practice
