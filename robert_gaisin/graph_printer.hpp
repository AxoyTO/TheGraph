#pragma once
#include <sstream>

#include "graph_traverser.hpp"

namespace uni_course_cpp {
class Graph;
struct GraphPath;

namespace graph_printing {
std::string print_graph(const Graph& graph);
std::string print_graph_description(const Graph& graph);
std::string print_path(const GraphPath& path);
}  // namespace graph_printing
}  // namespace uni_course_cpp
