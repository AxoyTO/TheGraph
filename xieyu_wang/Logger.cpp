#include "Logger.hpp"
#include <iostream>
#include "Config.hpp"
namespace uni_course_cpp {

Logger::Logger() : file_(config::loggerFilePath()) {
  if (!file_.is_open()) {
    throw std::runtime_error("Error to open/read log");
  }
}

void Logger::log(const std::string& string) {
  const std::lock_guard lock(mutex_);
  file_ << string;
  std::cout << string;
}

Logger::~Logger() {
  file_.close();
}

}  // namespace uni_course_cpp
