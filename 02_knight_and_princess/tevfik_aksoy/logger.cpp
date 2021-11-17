#include "logger.hpp"

void Logger::set_file(const std::string filename) {
  this->logfilename = filename;
}

void Logger::print_and_write(const std::string& string,
                             std::ostream& output_stream,
                             std::ofstream& file_stream) const {
  output_stream << string;
  file_stream << string;
}

void Logger::log(const std::string& string) const {
  std::ofstream logfile(logfilename, std::ofstream::app);
  assert(logfile.is_open() && "Error while opening the log file!");
  print_and_write(string, std::cout, logfile);
}