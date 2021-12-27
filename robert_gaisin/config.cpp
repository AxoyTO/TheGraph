
#include "config.hpp"
#include <string>
namespace uni_course_cpp {
namespace config {
std::string log_file_path() {
  return std::string(kTempDirectoryPath) + "log.txt";
}
}  // namespace config
}  // namespace uni_course_cpp
