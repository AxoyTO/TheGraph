#pragma once

#include <string>
#include "graph.hpp"

namespace uni_cource_cpp {
class GraphPrinter {
 public:
  explicit GraphPrinter(Graph& input_graph);
  std::string print() const;
  std::string print_graph_description() const;
  std::string print_vertex(const Vertex& vertex) const;
  std::string print_edge(const Edge& Edge) const;

 private:
  Graph& graph_;
};
}  // namespace uni_cource_cpp
