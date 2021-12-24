#pragma once

#include <fstream>
#include <string>

namespace uni_course_cpp {

class Logger {
 public:
  static Logger& get_instance() {
    static Logger logger;
    return logger;
  }

  void log(const std::string& string);

  ~Logger();

 private:
  std::ofstream file_stream_;

  Logger();

  Logger(const Logger&) = delete;
  Logger& operator=(const Logger&) = delete;
  Logger(Logger&&) = delete;
  Logger& operator=(Logger&&) = delete;
};

}  // namespace uni_course_cpp
