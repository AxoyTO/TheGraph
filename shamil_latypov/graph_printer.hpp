#pragma once

#include <string>
#include "graph.hpp"

namespace uni_cource_cpp {

std::string color_to_string(const Edge::Color& color);

class GraphPrinter {
 public:
  std::string print(const Graph& graph) const;
  std::string print_vertex(const Vertex& vertex) const;
  std::string print_edge(const Edge& edge) const;
};

}  // namespace uni_cource_cpp
