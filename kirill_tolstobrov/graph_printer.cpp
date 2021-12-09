#include "graph_printer.hpp"

#include <iostream>

namespace uni_cpp_practice {

std::string GraphPrinter::color_to_string(
    const uni_cpp_practice::Edge::Color& color) {
  switch (color) {
    case uni_cpp_practice::Edge::Color::Grey:
      return "grey";
    case uni_cpp_practice::Edge::Color::Green:
      return "green";
    case uni_cpp_practice::Edge::Color::Blue:
      return "blue";
    case uni_cpp_practice::Edge::Color::Yellow:
      return "yellow";
    case uni_cpp_practice::Edge::Color::Red:
      return "red";
  }
  throw std::runtime_error("Nonexistent color");
}

std::string GraphPrinter::print_vertex(const uni_cpp_practice::Vertex& vertex) {
  std::string result = "";
  auto edge_ids = vertex.get_edge_ids();
  result += "{\n\"id\": " + std::to_string(vertex.id);
  result += ",\n\"edge_ids\": [";

  for (int i = 0; i < edge_ids.size(); i++) {
    result += std::to_string(edge_ids[i]);
    if (i != edge_ids.size() - 1) {
      result += ", ";
    }
  }
  result += "],\n";

  result += "\"depth\": " + std::to_string(vertex.depth);
  result += "\n}";
  return result;
}

std::string GraphPrinter::print_edge(const uni_cpp_practice::Edge& edge) {
  std::string result = "";
  result += "{\n\"id\": " + std::to_string(edge.id);
  result += ",\n\"vertex_ids\": ";

  result += "[" + std::to_string(edge.vertex1_id);
  result += ", " + std::to_string(edge.vertex2_id);
  result += "],\n\"color\": \"";
  result += color_to_string(edge.color);
  result += "\"\n}";

  return result;
}

std::string GraphPrinter::print() const {
  std::string result = "{\n  \"vertices\": [\n";
  auto vertices = graph_.get_vertices();
  auto edges = graph_.get_edges();
  for (int i = 0; i < vertices.size(); i++) {
    result += print_vertex(vertices[i]);
    if (i != vertices.size() - 1)
      result += ",\n";
  }

  result += "\n  ],\n  \"edges\": [\n";

  for (int i = 0; i < edges.size(); i++) {
    result += print_edge(edges[i]);
    if (i != edges.size() - 1)
      result += ",\n";
  }

  result += "\n  ]\n}";
  return result;
}
}  // namespace uni_cpp_practice
