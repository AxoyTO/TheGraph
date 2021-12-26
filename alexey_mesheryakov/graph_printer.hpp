#pragma once
#include <string>
#include "graph.hpp"
namespace uni_course_cpp {
class GraphPrinter {
 public:
  explicit GraphPrinter(const Graph& graph) : graph_(graph){};
  std::string print() const;
  static std::string edge_color_to_string(const Edge::Color& color);

 private:
  const Graph& graph_;
};

}  // namespace uni_course_cpp
