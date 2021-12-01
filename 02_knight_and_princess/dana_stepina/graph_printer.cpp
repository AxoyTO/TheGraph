#include "graph_printer.hpp"

using Color = Edge::Color;

std::string GraphPrinter::print_vertex(const Vertex& vertex) const {
  std::string str_vertex = "\t  {\n\t\t\"id\":" + std::to_string(vertex.id) +
                           ",\n\t\t\"edge_ids\":[";
  for (const auto& edge_id : vertex.get_edge_ids()) {
    str_vertex += std::to_string(edge_id);
    if (edge_id != vertex.get_edge_ids().back())
      str_vertex += ",";
  }
  str_vertex += "],\n\t\t\"depth\":" + std::to_string(vertex.depth) + "\n\t  }";
  return str_vertex;
}

std::string GraphPrinter::print_edge(const Edge& edge) const {
  std::string str_edge =
      "\t  {\n\t\t\"id\":" + std::to_string(edge.id) +
      ",\n\t\t\"vertex_ids\":[" + std::to_string(edge.vertex_start) + "," +
      std::to_string(edge.vertex_end) + "],\n\t\t\"color\": ";
  switch (edge.color) {
    case Color::Grey:
      str_edge += "\"grey\"\n\t  }";
      break;
    case Color::Green:
      str_edge += "\"green\"\n\t  }";
      break;
    case Color::Yellow:
      str_edge += "\"yellow\"\n\t  }";
      break;
    case Color::Red:
      str_edge += "\"red\"\n\t  }";
      break;
  }
  return str_edge;
}

std::string GraphPrinter::print() const {
  std::string graph_string = "{";

  // write graph depth
  graph_string += "\n\t\"depth\": " + std::to_string(0) + ",";

  // write vertices
  graph_string += "\n\t\"vertices\": [\n";
  for (const auto& vertex : graph_.get_vertices()) {
    graph_string += print_vertex(vertex);
    if (vertex.id != graph_.get_vertices().back().id)
      graph_string += ",\n";
  }
  graph_string += "\n\t],";

  // write edges
  graph_string += "\n\t\"edges\": [\n";
  for (const auto& edge : graph_.get_edges()) {
    graph_string += print_edge(edge);
    if (edge.id != graph_.get_edges().back().id)
      graph_string += ",\n";
  }
  graph_string += "\n\t]";

  graph_string += "\n}\n";
  return graph_string;
}
