#pragma once

#include <string>
#include "graph.hpp"

namespace uni_cource_cpp {

class Logger {
 public:
  static Logger& get_logger() {
    static Logger logger;
    return logger;
  }

  void set_file_path();

  void log(const std::string& string);

  ~Logger() = default;

 private:
  Logger() = default;

  Logger(const Logger&) = delete;
  Logger& operator=(const Logger&) = delete;
  Logger(Logger&&) = delete;
  Logger& operator=(Logger&&) = delete;
};

}  // namespace uni_cource_cpp