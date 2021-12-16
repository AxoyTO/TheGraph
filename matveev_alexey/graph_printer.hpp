#include <string>
#include <vector>
#include "graph.hpp"

#pragma once

namespace uni_course_cpp {
class GraphPrinter {
 public:
  explicit GraphPrinter(const Graph& graph) : graph_(graph) {}
  std::string print() const;
  static std::string colorToString(const Edge::Colors& color);

 private:
  const Graph& graph_;
  std::string printVertex(const VertexId& id) const;
  std::string printEdge(const Edge& edge) const;
};
}  // namespace uni_course_cpp
