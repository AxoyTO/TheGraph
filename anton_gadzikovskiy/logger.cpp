#include "logger.hpp"
#include <fstream>
#include <iostream>

namespace uni_cource_cpp {

void Logger::log(const std::string& string) {
  file_stream_ << string << std::endl;
}

}  // namespace uni_cource_cpp
