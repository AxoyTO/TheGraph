#pragma once

#include <fstream>
#include <iostream>
#include <string>

namespace uni_cpp_practice {

class Graph;

class Logger {
 public:
  static Logger& get_logger(std::ofstream& f) {
    static Logger logger(f);
    return logger;
  }

  void log(std::string text) const {
    std::cout << text << std::endl;
    logfile_ << text;
  }

 private:
  std::ofstream& logfile_;

  Logger(std::ofstream& f) : logfile_(f) {}
  Logger(const Logger& root) = delete;
  Logger& operator=(const Logger&) = delete;
};

}  // namespace uni_cpp_practice
