#pragma once

#include <fstream>
#include <iostream>

namespace uni_cpp_practice {
class Logger {
 public:
  static Logger& get_instance() {
    static Logger instance;
    return instance;
  }

  void log(const std::string& string) const;

  void set_file(std::string filename);

 private:
  std::string filename_;

  Logger() = default;

  Logger(const Logger&) = delete;
  Logger& operator=(const Logger&) = delete;

  void print_and_write(const std::string& str,
                       std::ofstream& file_stream) const;
};
}  // namespace uni_cpp_practice
