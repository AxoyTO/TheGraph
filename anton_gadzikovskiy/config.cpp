#include "config.hpp"
#include <string>

namespace uni_cource_cpp {

namespace config {

std::string logger_file_path() {
  return std::string(uni_cource_cpp::config::kTempDirectoryPath) + "/log.txt";
}

}  // namespace config

}  // namespace uni_cource_cpp
