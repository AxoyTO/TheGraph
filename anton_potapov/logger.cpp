#include <iostream>

#include "logger.hpp"

void log(const std::string& string) {
    std::cout << string;
}

Logger& Logger::get_instance() {
  static Logger* logger = new Logger();
  return *logger;
}