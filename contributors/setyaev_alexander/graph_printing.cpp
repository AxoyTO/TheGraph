#include "graph_printing.hpp"
#include <array>
#include <iostream>
#include <sstream>

namespace uni_course_cpp {
namespace printing {

std::string color_to_string(const uni_course_cpp::Edge::Color& color) {
  switch (color) {
    case uni_course_cpp::Edge::Color::Grey:
      return "grey";

    case uni_course_cpp::Edge::Color::Yellow:
      return "yellow";

    case uni_course_cpp::Edge::Color::Green:
      return "green";

    case uni_course_cpp::Edge::Color::Red:
      return "red";
  }

  throw std::runtime_error("Failed to determine color");
}

std::string print_graph(const uni_course_cpp::Graph& graph) {
  std::stringstream graph_string;
  graph_string << "{\n  depth: " << graph.get_depth()
               << ",\n  vertices: {amount: " << graph.get_vertices().size()
               << ", distribution: [";
  for (int depth = 0; depth < graph.get_depth(); ++depth) {
    graph_string << graph.get_vertex_ids_at_depth(depth).size() << ", ";
  }
  graph_string << "]},\n  edges: {amount: " << graph.get_vertices().size()
               << ", distribution: {";
  const std::array<uni_course_cpp::Edge::Color, 4> colors = {
      uni_course_cpp::Edge::Color::Grey, uni_course_cpp::Edge::Color::Green,
      uni_course_cpp::Edge::Color::Yellow, uni_course_cpp::Edge::Color::Red};
  for (const auto& color : colors) {
    if (color != colors[0]) {
      graph_string << ", ";
    }
    graph_string << color_to_string(color) << ": "
                 << graph.get_colored_edge_ids(color).size();
  }

  graph_string << "}}\n}";
  return graph_string.str();
}

}  // namespace printing
}  // namespace uni_course_cpp
