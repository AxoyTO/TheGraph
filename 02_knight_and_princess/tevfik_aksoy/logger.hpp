#pragma once
#ifndef _LOGGER_
#define _LOGGER_

#include <cassert>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

class Logger {
 public:
  static Logger& get_instance() {
    static Logger instance;
    return instance;
  }

  void set_file(std::string filename = "./temp/log.txt");

  void log(const std::string& string) const;

 private:
  std::string logfilename;

  Logger() = default;

  Logger(const Logger&) = delete;
  Logger& operator=(const Logger&) = delete;
  Logger(Logger&&) = delete;
  Logger& operator=(Logger&&) = delete;

  void print_and_write(const std::string& str,
                       std::ostream& output_stream,
                       std::ofstream& file_stream) const;
};
#endif
