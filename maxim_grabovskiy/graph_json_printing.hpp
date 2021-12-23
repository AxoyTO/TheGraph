#pragma once
#include <sstream>
#include <string>
#include "graph.hpp"
namespace uni_course_cpp {
using std::string;

class GraphPrinter {
 public:
  explicit GraphPrinter(Graph const& graph) : graph_(graph) {}
  string print_vertex(Graph::Vertex const& vertex) const;
  string print_edge(Graph::Edge const& edge) const;
  string print() const;

 private:
  string printEdgeColor(Graph::Edge::Color const& color) const;
  Graph const& graph_;
};
}  // namespace uni_course_cpp
