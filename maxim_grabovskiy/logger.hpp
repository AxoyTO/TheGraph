#pragma once
#include <chrono>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include "config.hpp"

namespace uni_course_cpp {
class Logger {
 public:
  static Logger& getLogger() {
    static Logger* logger = new Logger();
    return *logger;
  }
  void log(const std::string& log_string);

 private:
  std::string get_current_date_time() {
    const auto date_time = std::chrono::system_clock::now();
    const auto date_time_t = std::chrono::system_clock::to_time_t(date_time);
    std::stringstream date_time_string;
    date_time_string << std::put_time(std::localtime(&date_time_t),
                                      "%Y.%m.%d %H:%M:%S");
    return date_time_string.str();
  }
};
}  // namespace uni_course_cpp
