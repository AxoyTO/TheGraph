#pragma once

#include <string>
#include "graph.hpp"

class GraphPrinter {
 public:
  explicit GraphPrinter(const Graph& input_graph);
  std::string print() const;

 private:
  const Graph& graph;
  std::string print_vertex(const Vertex& vertex) const;
  std::string print_edge(const Edge& Edge) const;
};
