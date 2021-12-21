#include "config.hpp"
#include <string>

namespace config {
const std::string getLogFilePath() {
  static const std::string LOG_FILE_PATH = TEMP_DIRECTORY_PATH + LOG_FILE_NAME;
  return LOG_FILE_PATH;
}
}  // namespace config
