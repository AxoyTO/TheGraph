#include "Config.hpp"
#include <string>

namespace uni_course_cpp {

namespace config {

std::string loggerFilePath() {
  return std::string(uni_course_cpp::config::TempDirectoryPath) + "/log.txt";
}

}  // namespace config

}  // namespace uni_course_cpp
