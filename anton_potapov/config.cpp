#include <string>

#include "config.hpp"

namespace uni_cource_cpp {
namespace config {
std::string log_file_path() {
  return std::string(config::TEMP_DIRECTORY_PATH) +
         std::string(config::LOG_FILENAME);
}
}  // namespace config
}  // namespace uni_cource_cpp
