#pragma once
#include <string>
#include "graph.hpp"
#include "graph_path.hpp"

namespace uni_course_cpp {
class GraphPrinter {
 public:
  explicit GraphPrinter(const Graph& graph) : graph_(graph){};
  std::string print() const;
  static std::string print_path(const GraphPath& path);
  static std::string edge_color_to_string(const Edge::Color& color);

 private:
  const Graph& graph_;
};

}  // namespace uni_course_cpp
