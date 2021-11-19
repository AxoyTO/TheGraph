#pragma once

#include <fstream>
#include <iostream>
#include <string>

constexpr int COLORS_NUMBER = 6;

class Graph;

class Logger {
 public:
  static Logger& get_logger(std::ofstream& f) {
    static Logger logger(f);
    return logger;
  }

  void log(std::string text) const {
    std::cout << text << std::endl;
    logfile_ << text;
  }

 private:
  std::ofstream& logfile_;

  Logger(std::ofstream& f) : logfile_(f) {}
  Logger(const Logger& root) = delete;
  Logger& operator=(const Logger&) = delete;
};

void write_log(Graph& work_graph,
               std::ofstream& logfile,
               const int& depth,
               const int& new_vertices_num,
               const int& graph_num,
               const Logger& logger);
