#include "config.hpp"
#include <string>

namespace uni_cource_cpp {

std::string config::logger_file_path() {
  return std::string(uni_cource_cpp::config::kTempDirectoryPath) + "/log.txt";
}

}  // namespace uni_cource_cpp
