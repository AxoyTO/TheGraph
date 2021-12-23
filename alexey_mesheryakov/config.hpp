#pragma once
#include <string>
namespace uni_course_cpp {
namespace config {

inline constexpr const char* kLogFilename = "log.txt";
inline constexpr const char* kTempDirectoryPath =
    "/home/alex/prac/graph_task/temp/";
inline const std::string kLogFilePath =
    std::string(kTempDirectoryPath) + std::string(kLogFilename);
static std::string log_file_path() {
  return kLogFilePath;
}
}  // namespace config
}  // namespace uni_course_cpp
