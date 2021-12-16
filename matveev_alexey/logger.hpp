#include <chrono>
#include <fstream>
#include <iostream>
#include <optional>
#include <string>
#include "config.hpp"
#include "graph.hpp"
#include "graph_printer.hpp"

#pragma once

namespace uni_course_cpp {
class Logger {
 public:
  void log(const std::string& string);

  ~Logger();

  static Logger& getLogger();

 private:
  Logger();
  std::ofstream file_stream_;
  Logger(const Logger&) = delete;
  Logger& operator=(const Logger&) = delete;
  Logger(Logger&&) = delete;
  Logger& operator=(Logger&&) = delete;
  std::string getCurrentDateTime();
};
}  // namespace uni_course_cpp
