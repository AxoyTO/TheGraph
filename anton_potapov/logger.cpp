#include <fstream>
#include <iostream>

#include "config.hpp"
#include "logger.hpp"

namespace uni_cource_cpp {
void Logger::log(const std::string& string) {
  std::cout << string;
  std::ofstream output_filestream;
  output_filestream.open(config::log_file_path(), std::ios_base::app);
  output_filestream << "something you want to add to your outputfile";
  output_filestream.close();
}

Logger& Logger::get_logger() {
  static Logger* logger = new Logger();
  return *logger;
}
}  // namespace uni_cource_cpp
