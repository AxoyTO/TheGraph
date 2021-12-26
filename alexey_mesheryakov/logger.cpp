#include "logger.hpp"
#include <chrono>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include "config.hpp"

namespace {
std::string get_current_date_time() {
  const auto date_time = std::chrono::system_clock::now();
  const auto date_time_t = std::chrono::system_clock::to_time_t(date_time);
  std::stringstream date_time_string;
  date_time_string << std::put_time(std::localtime(&date_time_t),
                                    "%Y.%m.%d %H:%M:%S");
  return date_time_string.str();
}
}  // namespace

namespace uni_course_cpp {
void Logger::log(const std::string& string) {
  const std::lock_guard lock(mutex_);
  const std::string log_string =
      get_current_date_time() + std::string(": ") + string;
  std::cout << log_string;
  file_stream_ << log_string;
}

Logger::~Logger() {
  if (file_stream_.is_open())
    file_stream_.close();
}

Logger::Logger() : file_stream_(config::kLogFilePath) {
  if (!file_stream_.is_open())
    throw std::runtime_error("Can't create file stream");
}
}  // namespace uni_course_cpp