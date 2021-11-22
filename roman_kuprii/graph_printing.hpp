#pragma once

#include <string>

namespace uni_cpp_practice {

class Graph;

namespace graph_printing {

std::string graph_to_json(const Graph& graph);
std::string vertex_to_json(const Vertex& graph);
std::string edge_to_json(const Graph& graph);

}  // namespace graph_printing

}  // namespace uni_cpp_practice
