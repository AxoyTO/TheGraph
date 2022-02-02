#include "graph_printer.hpp"
#include <sstream>

namespace {
std::string print_vertex(const uni_course_cpp::Vertex& vertex) {
  std::string res;
  res += "{\n      \"id\": ";
  res += std::to_string(vertex.id);
  res += ",\n      \"edge_ids\": [";

  auto edge_ids = vertex.get_edge_ids();

  if (!edge_ids.empty()) {
    for (const auto& edge_id : edge_ids) {
      res += std::to_string(edge_id);
      res += ", ";
    }
    res.pop_back();
    res.pop_back();
  }
  res += "],\n      \"depth\": ";
  res += std::to_string(vertex.depth);
  res += "\n    }";

  return res;
}

std::string print_edge(const uni_course_cpp::Edge& edge) {
  std::string res;

  res += "{\n      \"id\": ";
  res += std::to_string(edge.id);
  res += ",\n      \"vertex_ids\": [";
  res += std::to_string(edge.vertex1_id);
  res += ", ";
  res += std::to_string(edge.vertex2_id);
  res += "],\n      \"color\": \"";
  res += uni_course_cpp::graph_printing::print_edge_color(edge.color);
  res += "\"\n    }, ";

  return res;
}
}  // namespace

namespace uni_course_cpp {

namespace graph_printing {
std::string print_graph_description(const Graph& graph) {
  std::stringstream res;

  res << "{  \n";
  res << "  depth: " << graph.get_depth() << ",\n";
  res << "  vertices: {amount: " << graph.get_vertices().size()
      << ", distribution: [";

  const int depth = graph.get_depth();

  for (int current_depth = 0; current_depth <= depth; ++current_depth) {
    res << graph.get_vertex_ids_in_depth(current_depth).size();
    if (current_depth != depth) {
      res << ", ";
    }
  }
  res << "]},\n";
  res << "  edges: {amount: " << graph.get_edges().size()
      << ", distribution: {";
  const auto color_edge_count_pairs = graph.get_color_edge_count_pairs();

  for (const auto& pair : color_edge_count_pairs) {
    res << print_edge_color(pair.first) << ": " << pair.second;
    res << ", ";
  }
  res.seekp(-2, res.cur);
  res << "}";

  return res.str();
}

std::string print_edge_color(const Edge::Color& color) {
  switch (color) {
    case Edge::Color::Gray:
      return "gray";
    case Edge::Color::Green:
      return "green";
    case Edge::Color::Yellow:
      return "yellow";
    case Edge::Color::Red:
      return "red";
  }
  throw std::runtime_error("Invalid color value");
}

std::string print_graph(const Graph& graph) {
  std::string res;
  res += "{\n  \"depth\": ";
  res += std::to_string(graph.get_depth());
  res += ",\n  \"vertices\": [\n    ";

  auto vertices = graph.get_vertices();

  if (!vertices.empty()) {
    for (const auto& vertex : graph.get_vertices()) {
      res += print_vertex(vertex);
      res += ", ";
    }
    res.pop_back();
    res.pop_back();
  }

  res += "\n  ],\n  \"edges\": [\n    ";

  auto edges = graph.get_edges();

  if (!edges.empty()) {
    for (const auto& edge : graph.get_edges()) {
      res += print_edge(edge);
    }
    res.pop_back();
    res.pop_back();
  }

  res += "\n  ]\n}\n";

  return res;
}

std::string print_path(const GraphPath& path) {
  std::stringstream res;

  res << "{vertices: [";

  const auto& vertex_ids = path.get_vertex_ids();

  for (const auto& vertex_id : vertex_ids) {
    res << vertex_id;
    res << ", ";
  }
  if (vertex_ids.size()) {
    res.seekp(-2, res.cur);
  }

  res << "], distance: ";
  res << path.distance();
  res << "}";

  return res.str();
}

}  // namespace graph_printing

}  // namespace uni_course_cpp
