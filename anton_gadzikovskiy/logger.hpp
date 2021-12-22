#pragma once

#include <fstream>
#include <string>
#include "config.hpp"
#include "graph.hpp"

namespace uni_cource_cpp {

class Logger {
 public:
  static Logger& get_logger() {
    static Logger logger;
    return logger;
  }

  void log(const std::string& string);

  ~Logger() = default;

 private:
  std::ofstream file_stream_;
  Logger();

  Logger(const Logger&) = delete;
  Logger& operator=(const Logger&) = delete;
  Logger(Logger&&) = delete;
  Logger& operator=(Logger&&) = delete;
};

}  // namespace uni_cource_cpp
