#pragma once
#include <fstream>
#include <mutex>
#include <optional>
#include <string>
#include "graph.hpp"

namespace uni_cource_cpp {

class Logger {
 public:
  static Logger& get_logger() {
    static Logger logger;
    return logger;
  }

  void log(const std::string& text);

  void set_output_file_path(const std::optional<std::string>& file_path);

  ~Logger() = default;

 private:
  std::optional<std::ofstream> output_stream_;

  Logger() = default;
  Logger(const Logger& root) = delete;
  Logger& operator=(const Logger&) = delete;
  Logger(Logger&&) = delete;
  Logger& operator=(Logger&&) = delete;
  std::mutex logger_lock_;
};

}  // namespace uni_cource_cpp
