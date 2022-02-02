#pragma once

#include <fstream>
#include <iostream>
#include <optional>
#include <string>

namespace uni_cpp_practice {

class Graph;

class Logger {
 public:
  static Logger& get_logger() {
    static Logger logger;
    return logger;
  }

  void log(const std::string& text);

  void set_output(const std::optional<std::string>& file_path);

  ~Logger();

 private:
  std::optional<std::ofstream> file_stream_ = std::nullopt;

  Logger() = default;
  Logger(const Logger& root) = delete;
  Logger& operator=(const Logger&) = delete;
  Logger(Logger&&) = delete;
  Logger& operator=(Logger&&) = delete;
};

}  // namespace uni_cpp_practice
