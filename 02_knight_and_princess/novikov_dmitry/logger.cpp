#include "logger.hpp"
#include <assert.h>

namespace uni_cpp_practice {

void Logger::set_file(const std::string filename) {
  filename_ = filename;
}

void Logger::print_and_write(const std::string& string,
                             std::ofstream& file_out) const {
  std::cout << string;
  file_out << string;
}

void Logger::log(const std::string& string) const {
  std::ofstream log_file;
  log_file.open(filename_, std::fstream::out | std::fstream::app);
  assert(log_file.is_open() && "Error while opening the log file!");
  print_and_write(string, log_file);
}
}  // namespace uni_cpp_practice
