#pragma once

#include <fstream>
#include <iostream>
#include <string>

constexpr int COLORS_NUMBER = 5;

class Graph;

class Logger {
 public:
  static Logger& get_logger() {
    static Logger logger;
    return logger;
  }

  void log(std::ofstream& logfile, const std::string& text) const {
    std::cout << text << std::endl;
    logfile << text;
  }

 private:
  Logger() {}
  Logger(const Logger& root) = delete;
  Logger& operator=(const Logger&) = delete;
};

void write_log(Graph& work_graph,
               std::ofstream& logfile,
               const int& depth,
               const int& new_vertices_num,
               const int& graph_num,
               const Logger& logger);
