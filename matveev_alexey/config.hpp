#pragma once

#include <string>

namespace config {
const std::string TEMP_DIRECTORY_PATH = static_cast<std::string>("temp/");
const std::string LOG_FILE_NAME = static_cast<std::string>("log.txt");
const std::string getLogFilePath();
}  // namespace config
