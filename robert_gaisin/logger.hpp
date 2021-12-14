#pragma once

#include <fstream>
#include <optional>

namespace uni_cource_cpp {
namespace config {
std::string log_file_path();
}
class Logger {
 public:
  Logger() : file_stream_(uni_cource_cpp::config::log_file_path()) {
    if (!file_stream_->is_open()) {
      throw std::runtime_error("Failed to open file stream");
    }
  }
  static Logger& get_logger() {
    static Logger logger;
    return logger;
  }

  void log(const std::string& log_string);

  //  void set_file(const std::optional<std::string>& filename);

  ~Logger();

 private:
  std::optional<std::ofstream> file_stream_ = std::nullopt;

  Logger(const Logger& root) = delete;
  Logger& operator=(const Logger&) = delete;
  Logger(Logger&&) = delete;
  Logger& operator=(Logger&&) = delete;
};
}  // namespace uni_cource_cpp
