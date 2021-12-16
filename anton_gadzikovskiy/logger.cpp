#include "logger.hpp"
#include <fstream>
#include <iostream>

namespace uni_cource_cpp {

void Logger::set_file_path() {
  std::ofstream file;
  file.open("temp/log.txt");
  file.clear();
}

void Logger::log(const std::string& string) {
  std::ofstream file;
  file.open("temp/log.txt", std::fstream::app);
  file << string << std::endl;
}

}  // namespace uni_cource_cpp