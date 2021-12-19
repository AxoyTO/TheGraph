#pragma once
#include <fstream>
#include <mutex>
#include <optional>
#include <string>

namespace uni_cpp_practice {
class Logger {
 public:
  static Logger& getLogger() {
    static Logger logger;
    return logger;
  }
  void setFile(const std::optional<std::string>& filename);
  void log(const std::string& string);
  ~Logger();

 private:
  std::optional<std::ofstream> file_;
  std::mutex mutex_;

  Logger() = default;
  Logger(const Logger&) = delete;
  Logger& operator=(const Logger&) = delete;
  Logger(Logger&&) = delete;
  Logger& operator=(Logger&&) = delete;
};
}  // namespace uni_cpp_practice