#pragma once

#include <string>
#include "graph.hpp"
#include "graph_traverser.hpp"

namespace uni_cource_cpp {
class GraphPrinter {
 public:
  explicit GraphPrinter(Graph& input_graph);
  std::string print() const;
  std::string print_graph_description() const;
  std::string print_vertex(const Vertex& vertex) const;
  std::string print_edge(const Edge& Edge) const;
  static std::string print_paths(std::vector<GraphPath> paths);

 private:
  Graph& graph_;
};
}  // namespace uni_cource_cpp
