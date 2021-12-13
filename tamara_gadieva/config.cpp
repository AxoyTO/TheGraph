#include "config.hpp"

namespace uni_cource_cpp {
namespace config {
std::string log_file_path() {
  return std::string(TEMP_FOLDER_PATH) + std::string(LOG_FILENAME);
}
}  // namespace config
}  // namespace uni_cource_cpp
