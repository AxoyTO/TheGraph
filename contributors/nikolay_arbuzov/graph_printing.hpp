#pragma once

#include "graph.hpp"
#include "graph_path.hpp"

namespace uni_course_cpp {
namespace printing {

std::string print_edge_color(const Graph::Edge::Color& color);

std::string path_to_json(const GraphPath& path);

std::string print_graph(const Graph& graph);

}  // namespace printing
}  // namespace uni_course_cpp
