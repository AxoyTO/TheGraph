#include "logger.hpp"

#include <iostream>

namespace uni_cource_cpp {

void Logger::log(const std::string& log_string) {
  std::cout << log_string << std::endl;

  if (file_stream_.has_value())
    file_stream_.value() << log_string << std::endl;
}

Logger::~Logger() {
  if (file_stream_.has_value())
    file_stream_->close();
}
}  // namespace uni_cource_cpp
