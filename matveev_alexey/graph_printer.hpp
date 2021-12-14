#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <utility>
#include <vector>
#include "graph.hpp"

#pragma once

namespace uni_course_cpp {
class GraphPrinter {
 public:
  explicit GraphPrinter(const Graph& graph) : graph_(graph) {}
  std::string print() const;
  std::string printVertex(const VertexId& id) const;
  std::string printEdge(const Edge& edge) const;
  static std::string colorToString(const Edge::Colors& color);

 private:
  const Graph& graph_;
};
}  // namespace uni_course_cpp
