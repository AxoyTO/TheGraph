#include "graph_printer.hpp"
#include <sstream>
#include "graph.hpp"

namespace uni_cource_cpp {

std::string GraphPrinter::print_edge_color(const Edge::Color& color) const {
  switch (color) {
    case Edge::Color::Gray:
      return "gray";
    case Edge::Color::Green:
      return "green";
    case Edge::Color::Blue:
      return "blue";
    case Edge::Color::Yellow:
      return "yellow";
    case Edge::Color::Red:
      return "red";
  }
  throw std::runtime_error("Invalid color value");
}

std::string GraphPrinter::print(const Graph& graph) const {
  std::stringstream result;
  result << "{\n  \"depth\": " << graph.get_depth()
         << ",\n  \"vertices\": [\n    ";

  // Вывод вершин
  const auto& vertices = graph.get_vertices();
  if (vertices.size()) {
    result << print_vertex(vertices[0]);
    for (auto vertex_it = vertices.begin() + 1; vertex_it != vertices.end();
         vertex_it++) {
      result << ",\n    " << print_vertex(*vertex_it);
    }
  }

  result << "\n  ],\n  \"edges\": [\n    ";

  // Вывод рёбер
  const auto& edges = graph.get_edges();
  if (edges.size()) {
    result << print_edge(edges[0]);
    for (auto edge_it = edges.begin() + 1; edge_it != edges.end(); edge_it++) {
      result << ",\n    " << print_edge(*edge_it);
    }
  }

  result << "\n  ]\n}\n";

  return result.str();
}

std::string GraphPrinter::print_vertex(const Vertex& vertex) const {
  std::stringstream ss;
  ss << "{\n      \"id\": " << vertex.get_id() << ",\n      \"edge_ids\": [";

  if (vertex.get_edge_ids().size()) {
    ss << vertex.get_edge_ids()[0];
    for (auto edge_id_it = vertex.get_edge_ids().begin() + 1;
         edge_id_it != vertex.get_edge_ids().end(); edge_id_it++) {
      ss << ", " << *edge_id_it;
    }
  }
  ss << "],\n      \"depth\": " << vertex.depth << "\n    }";
  return ss.str();
}

std::string GraphPrinter::print_edge(const Edge& edge) const {
  std::stringstream ss;
  ss << "{\n      \"id\": " << edge.get_id() << ",\n      \"vertex_ids\": ["
     << edge.get_vertex1_id() << ", " << edge.get_vertex2_id()
     << "],\n      \"color\": \"" << print_edge_color(edge.get_color())
     << "\"\n    }";
  return ss.str();
}

}  // namespace uni_cource_cpp
