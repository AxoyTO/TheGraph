#include <fstream>
#include <iostream>

#include "config.hpp"
#include "logger.hpp"

namespace uni_cource_cpp {
void Logger::log(const std::string& string) {
  std::cout << string;
  static std::ofstream output_filestream;
  output_filestream.open(config::log_file_path(), std::ios_base::app);
  output_filestream << string;
  output_filestream.close();
}

Logger& Logger::get_logger() {
  static Logger logger;
  return logger;
}
}  // namespace uni_cource_cpp
