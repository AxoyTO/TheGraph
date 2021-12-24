#pragma once
#include <chrono>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
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
class Logger {
 public:
  static Logger& getLogger() {
    static Logger logger;
    return logger;
  }
  ~Logger();
  void log(const std::string& log_string);

 private:
  Logger();
  Logger(const Logger&) = delete;
  Logger& operator=(const Logger&) = delete;
  std::ofstream file_;
  Logger(Logger&&) = delete;
  Logger& operator=(Logger&&) = delete;
};
}  // namespace uni_course_cpp
