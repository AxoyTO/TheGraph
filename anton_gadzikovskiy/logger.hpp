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
  Logger() : file_stream_(config::logger_file_path()) {
    if (!file_stream_.is_open()) {
      throw std::runtime_error("...");
    }
    file_stream_.clear();
  }

  Logger(const Logger&) = delete;
  Logger& operator=(const Logger&) = delete;
  Logger(Logger&&) = delete;
  Logger& operator=(Logger&&) = delete;
};

}  // namespace uni_cource_cpp
   // namespace uni_cource_cpp