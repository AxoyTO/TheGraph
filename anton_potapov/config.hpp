#pragma once

#include <string>

namespace uni_cource_cpp {
namespace config {
constexpr const char* TEMP_DIRECTORY_PATH = "./temp/";
constexpr const char* LOG_FILENAME = "log.txt";
std::string log_file_path();
}  // namespace config
}  // namespace uni_cource_cpp
