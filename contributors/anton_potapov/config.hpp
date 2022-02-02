#pragma once

#include <string>
#include <thread>

namespace uni_cource_cpp {
namespace config {
constexpr const char* TEMP_DIRECTORY_PATH = "./temp/";
constexpr const char* LOG_FILENAME = "log.txt";
const int MAX_THREADS_COUNT = std::thread::hardware_concurrency();
std::string log_file_path();
}  // namespace config
}  // namespace uni_cource_cpp
