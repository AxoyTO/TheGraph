#include "Logger.hpp"
#include <iostream>
#include "Config.hpp"
namespace uni_course_cpp {

Logger::Logger() : file_(config::loggerFilePath()) {
  if (!file_->is_open()) {
    throw std::runtime_error("Error to open/read log");
  }
  file_->clear();
}

void Logger::log(const std::string& string) {
  const std::lock_guard lock(mutex_);
  if (file_.has_value())
    file_.value() << string;
  std::cout << string;
}

Logger::~Logger() {
  if (file_.has_value())
    if (file_->is_open()) {
      file_->close();
      file_ = std::nullopt;
    }
}

}  // namespace uni_course_cpp
