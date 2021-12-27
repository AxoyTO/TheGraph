#pragma once
#include <string>
#include "graph.hpp"

namespace uni_course_cpp {
namespace printing {
namespace json {
std::string color_to_string(const uni_course_cpp::Edge::Color& color);
std::string graph_to_string(const uni_course_cpp::Graph& graph);
std::string vertex_to_string(
    const uni_course_cpp::Vertex& vertex,
    uni_course_cpp::Graph::Depth depth,
    const std::vector<uni_course_cpp::EdgeId>& connected_edges);
std::string edge_to_string(const uni_course_cpp::Edge& edge);
}  // namespace json
}  // namespace printing
}  // namespace uni_course_cpp
