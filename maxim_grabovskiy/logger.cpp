#include "logger.hpp"
#include <stdexcept>
#include <string>
namespace uni_course_cpp {

Logger::Logger() : file_(config::getLoggerFilePath()) {
  if (!file_.is_open()) {
    throw std::runtime_error("File error");
  }
}

Logger::~Logger() {
  file_.close();
}

void Logger::log(const std::string& string) {
  std::cout << get_current_date_time() << " " << string << "\n";
  file_ << get_current_date_time() << " " << string << "\n";
}
}  // namespace uni_course_cpp
