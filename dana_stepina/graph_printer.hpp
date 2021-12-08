#pragma once

#include "graph.hpp"

#include <string>

class GraphPrinter {
 public:
  explicit GraphPrinter(const Graph& graph) : graph_(graph) {}

  std::string print() const;
  std::string print_vertex(const Vertex& vertex) const;
  std::string print_edge(const Edge& edge) const;

 private:
  const Graph& graph_;
};
