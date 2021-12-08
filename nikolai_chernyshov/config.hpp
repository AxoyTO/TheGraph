#pragma once

#include <fstream>

namespace config {
constexpr const char* k_temp_directory_path = "./temp/";
constexpr const char* k_log_filename = "log.txt";
std::string log_file_path() {
  std::stringstream res;
  res << k_temp_directory_path << k_log_filename;
  return res.str();
};
}  // namespace config
