#pragma once

#include <fstream>
#include <mutex>
#include <optional>
#include <string>

namespace uni_cource_cpp {
class Logger {
 public:
  static Logger& get_logger() {
    static Logger logger;
    return logger;
  }

  void log(const std::string& log_string);

  void set_file(const std::optional<std::string>& filename);

  ~Logger();

 private:
  std::optional<std::ofstream> file_stream_ = std::nullopt;

  Logger() = default;
  Logger(const Logger& root) = delete;
  Logger& operator=(const Logger&) = delete;
  Logger(Logger&&) = delete;
  Logger& operator=(Logger&&) = delete;

  std::mutex mutex_logger_;
};
}  // namespace uni_cource_cpp
