#pragma once

#include <string>

#include "graph_traverser.hpp"

namespace uni_cpp_practice {

class Graph;

namespace graph_printing {

std::string color_to_string(const Edge::Color& color);

std::string graph_to_json(const Graph& graph);
std::string vertex_to_json(const Vertex& graph);
std::string edge_to_json(const Graph& graph);

std::string path_to_json(const GraphTraverser::Path& path);

}  // namespace graph_printing

}  // namespace uni_cpp_practice
