#include <exception>
#include <fstream>
#include <iostream>
#include <mutex>

#include "config.hpp"
#include "logger.hpp"

namespace uni_cource_cpp {
Logger::Logger() : output_filestream_(config::log_file_path()) {
  const std::lock_guard logger_usage_lock(logger_use_mutex_);
  if (!output_filestream_.is_open()) {
    throw std::runtime_error("can't open log file");
  }
}

void Logger::log(const std::string& string) {
  const std::lock_guard logger_usage_lock(logger_use_mutex_);
  std::cout << string;
  output_filestream_ << string;
}

Logger& Logger::get_logger() {
  static Logger logger;
  return logger;
}
}  // namespace uni_cource_cpp
