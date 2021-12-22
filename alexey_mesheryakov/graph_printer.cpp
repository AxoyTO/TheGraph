#include "graph_printer.hpp"
#include <array>
#include <sstream>
#include <string>
namespace {
using uni_course_cpp::Edge;
using uni_course_cpp::Graph;
using uni_course_cpp::Vertex;

std::string edge_color_to_string(const Edge::Color& color) {
  switch (color) {
    case Edge::Color::Gray:
      return "gray";
    case Edge::Color::Green:
      return "green";
    case Edge::Color::Red:
      return "red";
    case Edge::Color::Blue:
      return "blue";
    case Edge::Color::Yellow:
      return "yellow";
  }
  throw std::runtime_error("Color doesn't exist");
}

std::string edge_to_string(const Edge& edge) {
  std::stringstream buffer;
  buffer << "{\"id\":" << edge.id << ",\"vertex_ids\":["
         << edge.vertex_ids.first << "," << edge.vertex_ids.second
         << "],\"color\":\"" << edge_color_to_string(edge.color) << "\"}";
  return buffer.str();
}

std::string vertex_to_string(const Vertex& vertex) {
  std::stringstream buffer;
  buffer << "{\"id\":" << vertex.id << ",\"edge_ids\":[";
  const auto& edge_ids = vertex.get_edge_ids();
  for (int i = 0; i < edge_ids.size() - 1; i++)
    buffer << edge_ids[i] << ",";
  buffer << edge_ids[edge_ids.size() - 1] << "],\"depth\":" << vertex.depth
         << "}";
  return buffer.str();
}
}  // namespace
namespace uni_course_cpp {
std::string GraphPrinter::print() const {
  std::stringstream buffer;
  const auto& vertices = graph_.get_vertices();
  const auto& edges = graph_.get_edges();
  buffer << "{\"depth\":" << graph_.get_depth() << ",\"vertices\":[";
  for (unsigned int i = 0; i < vertices.size(); i++) {
    const auto& vertex = vertices[i];
    buffer << vertex_to_string(vertex);
    if (i != vertices.size() - 1)
      buffer << ",";
  }
  buffer << "],\"edges\":[";
  for (unsigned int i = 0; i < edges.size(); i++) {
    const auto& edge = edges[i];
    buffer << edge_to_string(edge);
    if (i != edges.size() - 1)
      buffer << ",";
  }
  buffer << "]}";
  return buffer.str();
}

namespace printing {

std::string print_graph(const uni_course_cpp::Graph& graph) {
  std::stringstream log_string;
  log_string << "  depth: " << graph.get_depth() << ",\n";
  log_string << "  vertices: " << graph.get_vertices().size() << ", [";

  for (uni_course_cpp::Depth depth = 0; depth < graph.get_depth() - 1; depth++)
    log_string << graph.get_vertex_ids_at_depth(depth).size() << ", ";
  log_string << graph.get_vertex_ids_at_depth(graph.get_depth() - 1).size()
             << "],\n"
             << "  edges: " << graph.get_edges().size() << ", {";

  const auto colors = std::array<uni_course_cpp::Edge::Color, 5>{
      uni_course_cpp::Edge::Color::Gray, uni_course_cpp::Edge::Color::Green,
      uni_course_cpp::Edge::Color::Blue, uni_course_cpp::Edge::Color::Yellow,
      uni_course_cpp::Edge::Color::Red};
  for (const auto& color : colors) {
    log_string << edge_color_to_string(color) << ": "
               << graph.get_count_of_colored_edges(color);
    if (&color != colors.end() - 1) {
      log_string << ", ";
    }
  }
  log_string << "}\n}\n";
  return log_string.str();
}

}  // namespace printing
}  // namespace uni_course_cpp