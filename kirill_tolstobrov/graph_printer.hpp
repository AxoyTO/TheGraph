#pragma once

#include "graph.hpp"
#include "graph_path.hpp"

#include <iostream>

namespace uni_cpp_practice {
class GraphPrinter {
 public:
  explicit GraphPrinter(const Graph& graph) : graph_(graph) {}
  static std::string color_to_string(
      const uni_cpp_practice::Edge::Color& color);
  static std::string print_vertex(const uni_cpp_practice::Vertex& vertex);
  static std::string print_edge(const uni_cpp_practice::Edge& edge);
  std::string print() const;
  static std::string print_path(const GraphPath& path);
  static std::string print_paths(const std::vector<GraphPath>& paths);

 private:
  const Graph& graph_;
};
}  // namespace uni_cpp_practice
