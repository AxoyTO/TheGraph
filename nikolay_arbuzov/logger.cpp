#include "logger.hpp"
#include <fstream>
#include <iostream>
#include <string>
#include "config.hpp"

namespace uni_course_cpp {

void Logger::log(const std::string& string) {
  log_file << string;
  std::cout << string << std::endl;
}

}  // namespace uni_course_cpp
