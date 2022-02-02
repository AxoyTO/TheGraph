#include "logger.hpp"
#include <chrono>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <mutex>
#include <sstream>
#include <string>
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
  const std::lock_guard<std::mutex> lock(log_mutex_);
  const auto log_string = get_current_date_time() + " " + string;
  log_file_ << log_string;
  std::cout << log_string;
}

Logger::Logger() : log_file_(config::kLogFilePath) {
  if (!log_file_.is_open())
    throw std::runtime_error("File was not open");
}

}  // namespace uni_course_cpp
