#include <string>

#include "config.hpp"

namespace config {
std::string log_file_path() {
  return std::string(config::TEMP_DIRECTORY_PATH) +
         std::string(config::LOG_FILENAME);
}
}  // namespace config
