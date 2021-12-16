#pragma once

#include <string>
#include "graph.hpp"

namespace uni_cource_cpp {

using std::string;

namespace graph_printing {

string print_edge_color(const Graph::Edge::Color& color);
string print_vertex(const Graph::Vertex& vertex);
string print_edge(const Graph::Edge& edge);
string print_graph(const Graph& graph);
string print_graph_description(const Graph& graph);
}  // namespace graph_printing

}  // namespace uni_cource_cpp