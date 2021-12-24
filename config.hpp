#pragma once
#include <string>
namespace uni_course_cpp {
namespace config {

inline constexpr const char* kTempDirectoryPath = "./temp/";
inline constexpr const char* kLogFilename = "log.txt";
inline constexpr const char* loggerFilePath = "./temp/";
static std::string getLoggerFilePath() {
  return (std::string(loggerFilePath) + std::string(kLogFilename));
}
}  // namespace config
}  // namespace uni_course_cpp
