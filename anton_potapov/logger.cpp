#include <iostream>

#include "logger.hpp"

namespace uni_cource_cpp {
void Logger::log(const std::string& string) {
  std::cout << string;
}

Logger& Logger::get_instance() {
  static Logger* logger = new Logger();
  return *logger;
}
}  // namespace uni_cource_cpp
