#pragma once

#include <string>
#include "graph.hpp"

namespace uni_cource_cpp {

class GraphPrinter {
 public:
  std::string print_edge_color(const Edge::Color& color) const;
  std::string print(const Graph& graph) const;
  std::string print_vertex(const Vertex& vertex) const;
  std::string print_edge(const Edge& edge) const;
};

}  // namespace uni_cource_cpp
