#pragma once

#include <fstream>
#include <mutex>
#include <string>
#include "config.hpp"

namespace uni_course_cpp {

class Logger {
 public:
  static Logger& get_logger() {
    static Logger singleton_logger;
    return singleton_logger;
  }
  void log(const std::string& string);

 private:
  std::ofstream log_file_;
  std::mutex log_mutex_;

  Logger();
  ~Logger() { log_file_.close(); }
  Logger(const Logger&) = delete;
  Logger& operator=(const Logger&) = delete;
  Logger(Logger&&) = delete;
  Logger& operator=(Logger&&) = delete;
};

}  // namespace uni_course_cpp
