#pragma once

#include <string>
#include "graph.hpp"

namespace uni_course_cpp {

namespace graph_printing {
std::string print_graph_description(const Graph& graph);
std::string print_edge_color(const Edge::Color& color);
std::string print_graph(const Graph& graph);
}  // namespace graph_printing

}  // namespace uni_course_cpp
