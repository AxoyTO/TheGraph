#pragma once

#include <string>
#include "graph.hpp"
#include "graph_path.hpp"

namespace uni_cource_cpp {

namespace graph_printing {

std::string print_edge_color(const Graph::Edge::Color& color);
std::string print_vertex(const Graph::Vertex& vertex);
std::string print_edge(const Graph::Edge& edge);
std::string print_graph(const Graph& graph);
std::string print_graph_description(const Graph& graph);
std::string print_path(const GraphPath& path);

}  // namespace graph_printing

}  // namespace uni_cource_cpp
