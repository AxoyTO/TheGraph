#pragma once
#include <fstream>
#include <mutex>
#include <string>

namespace uni_cource_cpp {
class Logger {
 public:
  static Logger& get_instance() {
    static Logger instance;
    return instance;
  }

  void log(const std::string& string);

 private:
  std::ofstream file_stream_;
  std::mutex mutex_;

  Logger();
  ~Logger() = default;
  Logger(const Logger&) = delete;
  Logger& operator=(const Logger&) = delete;
  Logger(Logger&&) = delete;
  Logger& operator=(Logger&&) = delete;
};
}  // namespace uni_cource_cpp
