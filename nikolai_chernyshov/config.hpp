#pragma once

#include <fstream>
#include <sstream>

namespace uni_course_cpp {

namespace config {
constexpr const char* K_TEMP_DIRECTORY_PATH = "./temp/";
constexpr const char* K_LOG_FILENAME = "log.txt";
static std::string log_file_path() {
  std::stringstream res;
  res << K_TEMP_DIRECTORY_PATH << K_LOG_FILENAME;
  return res.str();
}

}  // namespace config

}  // namespace uni_course_cpp
