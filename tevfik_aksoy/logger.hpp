#pragma once

#include <fstream>
#include <mutex>
#include <optional>
#include <string>

namespace uni_cpp_practice {
class Logger {
 public:
  static Logger& get_instance() {
    static Logger instance;
    return instance;
  }

  void set_file(const std::optional<std::string>& filename);

  void log(const std::string& string);

  ~Logger();

 private:
  std::optional<std::ofstream> file_stream_;
  std::mutex mutex_;

  Logger() = default;
  Logger(const Logger&) = delete;
  Logger& operator=(const Logger&) = delete;
  Logger(Logger&&) = delete;
  Logger& operator=(Logger&&) = delete;
};
}  // namespace uni_cpp_practice
