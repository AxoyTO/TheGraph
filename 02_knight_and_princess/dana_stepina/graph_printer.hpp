#pragma once

#ifndef GRAPH_PRINTER_HPP
#define GRAPH_PRINTER_HPP

#include <iostream>
#include <string>
#include "graph.hpp"

class Graph_printer {
 public:
  void add_graph_string(const Graph& graph);
  const void print_graph_string() const {
    std::cout << graph_string_ << std::endl;
  }
  const std::string& get_graph_string() const { return graph_string_; }

 private:
  std::string graph_string_;

  std::string add_vertex_string(const Vertex& vertex);
  std::string add_edge_string(const Edge& edge);
};

#endif
