#pragma once

#include "graph.hpp"

#include <string>

namespace uni_cource_cpp {
class GraphPrinter {
 public:
  static std::string print_edge_color(const Graph::Edge::Color& color);
  static std::string print_graph_description(const Graph& graph);

  explicit GraphPrinter(const Graph& graph) : graph_(graph) {}

  std::string print() const;
  std::string print_vertex(const Graph::Vertex& vertex) const;
  std::string print_edge(const Graph::Edge& edge) const;

 private:
  const Graph& graph_;
};
}  // namespace uni_cource_cpp
