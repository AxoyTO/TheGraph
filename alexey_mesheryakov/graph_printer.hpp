#pragma once
#include <sstream>
#include <string>
#include "graph.hpp"

class GraphPrinter {
 public:
  explicit GraphPrinter(const Graph& graph) : graph_(graph){};
  std::string edge_to_string(const Graph::Edge& edge) const;
  std::string vertex_to_string(const Graph::Vertex& vertex) const;
  std::string print() const;

 private:
  const Graph& graph_;
};
