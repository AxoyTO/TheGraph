#include "Logger.hpp"
#include <iostream>

namespace uni_cpp_practice {
void Logger::setFile(const std::optional<std::string>& filename) {
  if (!filename.has_value()) {
    if (file_.has_value()) {
      file_->close();
      file_ = std::nullopt;
    }
    return;
  }

  if (file_.has_value()) {
    file_->close();
  }

  file_ = std::ofstream(filename.value());
  if (!file_->is_open())
    throw std::runtime_error("Unable to open file");
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

}  // namespace uni_cpp_practice
