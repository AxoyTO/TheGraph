#pragma once

#include <chrono>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
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
  std::ofstream log_file;

  Logger() : log_file(config::kLogFilePath) {
    if (!log_file.is_open())
      throw std::runtime_error("File was not open");
  }
  ~Logger() { log_file.close(); }
  Logger(const Logger&) = delete;
  Logger& operator=(const Logger&) = delete;
  Logger(Logger&&) = delete;
  Logger& operator=(Logger&&) = delete;
};

}  // namespace uni_course_cpp
