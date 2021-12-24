#pragma once
#include "graph.hpp"
#include "graph_traverser.hpp"

using uni_cource_cpp::Edge;
using uni_cource_cpp::Graph;
using uni_cource_cpp::GraphTraverser;
using uni_cource_cpp::Vertex;

namespace graph_printing {
std::string color_to_string(const Edge::Color& color);
std::string vertices_distribution(const Graph& graph);
std::string edges_distribution(const Graph& graph);
std::string graph_description(const Graph& graph);
std::string graph_to_json_string(const Graph& graph);
std::string vertex_to_json_string(const Vertex& vertex);
std::string edge_to_json_string(const Edge& edge);
std::string print_path(const GraphTraverser::GraphPath& path);
}  // namespace graph_printing
