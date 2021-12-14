#include <chrono>
#include <fstream>
#include <iostream>
#include <optional>
#include <string>
#include "graph.hpp"
#include "graph_printer.hpp"

#pragma once

namespace uni_course_cpp {
class Logger {
 public:
  void log(const std::string& string);

  void setFile(std::optional<std::string> file_path);

  ~Logger();

  static Logger& getLogger();

 private:
  Logger() = default;
  std::optional<std::ofstream> file_stream;
  Logger(const Logger&) = delete;
  Logger& operator=(const Logger&) = delete;
  Logger(Logger&&) = delete;
  Logger& operator=(Logger&&) = delete;
  std::string getCurrentDateTime();
};
}  // namespace uni_course_cpp
