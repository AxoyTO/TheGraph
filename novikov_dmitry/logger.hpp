#pragma once

#include <fstream>
#include <optional>
#include <string>

namespace uni_cpp_practice {
class Logger {
 public:
  static Logger& get_instance() {
    static Logger instance;
    return instance;
  }

  void log(const std::string& string);

  void set_file(const std::optional<std::string>& filename);

  ~Logger();

 private:
  std::optional<std::ofstream> file_stream_;

  Logger() = default;
  Logger(const Logger&) = delete;
  Logger& operator=(const Logger&) = delete;
  Logger(Logger&&) = delete;
  Logger& operator=(Logger&&) = delete;
};
}  // namespace uni_cpp_practice
