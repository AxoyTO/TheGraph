#include "graph_printer.hpp"

namespace {

std::string print_vertex(const uni_cpp_practice::Vertex& vertex) {
  std::string json_string;
  json_string +=
      "\t{ \"id\": " + std::to_string(vertex.id) + ", \"edge_ids\": [";
  for (int i = 0; i < vertex.get_edge_ids().size(); i++) {
    json_string += std::to_string(vertex.get_edge_ids()[i]);
    if (i + 1 != vertex.get_edge_ids().size())
      json_string += ", ";
  }
  json_string += "], \"depth\": " + std::to_string(vertex.depth) + "}";
  return json_string;
}

std::string print_edge(const uni_cpp_practice::Edge& edge) {
  std::string json_string;
  json_string += "\t{ \"id\": " + std::to_string(edge.id) +
                 ", \"vertex_ids\": [" + std::to_string(edge.source) + ", " +
                 std::to_string(edge.destination) + R"(], "color": ")" +
                 color_to_string(edge.color) + "\" }";
  return json_string;
}

}  // namespace

namespace uni_cpp_practice {
std::string GraphPrinter::print() const {
  std::string json_string;
  json_string += "{\n\"vertices\": [\n";
  for (int i = 0; i < graph_.get_vertices().size(); i++) {
    json_string += print_vertex(graph_.get_vertices()[i]);
    if (i + 1 != graph_.get_vertices().size())
      json_string += ",\n";
  }
  json_string += "\n  ],\n";

  json_string += "\"edges\": [\n";
  for (int i = 0; i < graph_.get_edges().size(); i++) {
    json_string += print_edge(graph_.get_edges()[i]);
    if (i + 1 != graph_.get_edges().size())
      json_string += ",\n";
  }
  json_string += "\n  ]\n}\n";
  return json_string;
}
}  // namespace uni_cpp_practice
