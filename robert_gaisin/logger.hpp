#pragma once

#include <fstream>
#include <mutex>
#include <optional>

#include "config.hpp"

namespace uni_cource_cpp {

class Logger {
 public:
  static Logger& get_logger() {
    static Logger logger;
    return logger;
  }

  void log(const std::string& log_string);

  ~Logger();

 private:
  Logger() : file_stream_(uni_cource_cpp::config::log_file_path()) {
    if (!file_stream_.is_open()) {
      throw std::runtime_error("Failed to open file stream");
    }
  }
  std::mutex log_mutex;

  std::ofstream file_stream_;

  Logger(const Logger& root) = delete;
  Logger& operator=(const Logger&) = delete;
  Logger(Logger&&) = delete;
  Logger& operator=(Logger&&) = delete;
};
}  // namespace uni_cource_cpp
