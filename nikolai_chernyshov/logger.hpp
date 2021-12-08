#pragma once

#include <fstream>
#include <optional>
#include <string>

namespace uni_course_cpp {

class Logger {
 public:
  static Logger& get_instance() {
    static Logger logger;
    return logger;
  }

  void set_file(const std::optional<std::string>& filename);

  void log(const std::string& string);

  ~Logger();

 private:
  // можем указывать или не указывать файл вывода
  std::optional<std::ofstream> file_stream_;

  Logger() = default;

  Logger(const Logger&) = delete;
  Logger& operator=(const Logger&) = delete;
  Logger(Logger&&) = delete;
  Logger& operator=(Logger&&) = delete;
};

}  // namespace uni_course_cpp
