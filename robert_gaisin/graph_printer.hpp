#pragma once
#include <sstream>
#include "graph.hpp"
#include "graph_traverser.hpp"

namespace uni_cource_cpp {

using uni_course_cpp::GraphPath;

namespace graph_printing {
std::string print_graph(const Graph& graph);
std::string print_graph_description(const Graph& graph);
std::string print_path(const GraphPath& path);
}  // namespace graph_printing
}  // namespace uni_cource_cpp
