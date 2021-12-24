#pragma once
#include <sstream>
#include <string>
#include "graph.hpp"
namespace uni_course_cpp {
class GraphJsonPrinter {
 public:
  explicit GraphJsonPrinter(Graph const& graph) : graph_(graph) {}
  std::string print_vertex(Graph::Vertex const& vertex) const;
  std::string print_edge(Graph::Edge const& edge) const;
  std::string print() const;
  static std::string printEdgeColor(Graph::Edge::Color const& color);

 private:
  Graph const& graph_;
};
}  // namespace uni_course_cpp
