#pragma once

#include <fstream>
#include <sstream>

namespace uni_course_cpp {

namespace config {
constexpr const char* TEMP_DIRECTORY_PATH = "./temp/";
constexpr const char* LOG_FILENAME = "log.txt";
static std::string log_file_path() {
  std::stringstream res;
  res << TEMP_DIRECTORY_PATH << LOG_FILENAME;
  return res.str();
}

}  // namespace config

}  // namespace uni_course_cpp
