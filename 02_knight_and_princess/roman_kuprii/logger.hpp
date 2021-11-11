#pragma once

#include <fstream>
#include <iostream>
#include <string>
#include <memory>

constexpr int COLORS_NUMBER = 5;

class Graph;

class Logger {
 public:
  static Logger& get_logger() {
    static Logger logger;
    return logger;
  }

  void write(std::ofstream& logfile, const std::string log) const {
    std::cout << log << std::endl;
    logfile << log;
  }

 private:
  Logger() {}
  Logger(const Logger& root) = delete;
  Logger& operator=(const Logger&) = delete;
  static std::shared_ptr<Logger> log;
};

void write_log(Graph& work_graph,
               std::ofstream& logfile,
               const int& depth,
               const int& new_vertices_num,
               const int& graph_num,
               const Logger& logger);
