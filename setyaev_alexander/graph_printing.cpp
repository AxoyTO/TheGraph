#include "graph_printing.hpp"
#include <iostream>
#include <sstream>

namespace uni_course_cpp {
namespace printing {

int get_colored_edges_count(const uni_course_cpp::Graph& graph,
                            const uni_course_cpp::Edge::Color& color) {
  int count = 0;
  for (const auto& edge : graph.get_edges()) {
    if (edge.get_color() == color) {
      ++count;
    }
  }
  return count;
}

std::string print_graph(const uni_course_cpp::Graph& graph) {
  std::stringstream graph_string;
  graph_string << "{\n  depth: " << graph.get_depth()
               << ",\n  vertices: {amount: " << graph.get_vertices().size()
               << ", distribution: [";
  for (int depth = 0; depth < graph.get_depth(); ++depth) {
    graph_string << graph.get_vertex_ids_at_depth(depth).size() << ", ";
  }
  graph_string
      << "]},\n  edges: {amount: " << graph.get_vertices().size()
      << ", distribution: {grey: "
      << get_colored_edges_count(graph, uni_course_cpp::Edge::Color::Grey)
      << ", green: "
      << get_colored_edges_count(graph, uni_course_cpp::Edge::Color::Green)
      << ", yellow: "
      << get_colored_edges_count(graph, uni_course_cpp::Edge::Color::Yellow)
      << ", red: "
      << get_colored_edges_count(graph, uni_course_cpp::Edge::Color::Red)
      << "}}\n}";
  return graph_string.str();
}
std::string generation_started_string(int graph_number) {
  std::stringstream output;
  output << "Graph " << graph_number << ", GenerationStarted";
  return output.str();
}
std::string generation_finished_string(int graph_number,
                                       const std::string& graph_description) {
  std::stringstream output;
  output << "Graph " << graph_number << ", GenerationFinished "
         << graph_description;
  return output.str();
}
}  // namespace printing
}  // namespace uni_course_cpp
