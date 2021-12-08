#pragma once

#include <string>
#include "graph.hpp"

namespace uni_course_cpp {

namespace graph_printing {
std::string print_graph_description(const Graph& graph);
}  // namespace graph_printing

std::string color_to_string(const Edge::Color& color);

class GraphPrinter {
 public:
  std::string to_json(const Graph& graph) const;
};

}  // namespace uni_course_cpp
