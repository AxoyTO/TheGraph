#include "graph_printer.hpp"

#include <array>
#include <iostream>
#include <sstream>

using Color = uni_cource_cpp::Graph::Edge::Color;

namespace {
constexpr int NUM_COLORS = 4;
}

namespace uni_cource_cpp {
std::string GraphPrinter::print_graph_description(const Graph& graph) {
  std::stringstream log_string;

  const Depth depth = graph.get_depth();

  log_string << "  depth: " << depth << ",\n";

  log_string << "  vertices: " << graph.get_vertices().size() << ", [";
  for (Depth current_depth = 0; current_depth <= depth; ++current_depth) {
    log_string << graph.get_vertex_ids_at(depth).size();
    if (current_depth != depth)
      log_string << ", ";
  }
  log_string << "],\n";

  const std::array<Color, NUM_COLORS> colors_of_edges = {
      Color::Grey, Color::Green, Color::Yellow, Color::Red};
  log_string << "  edges: " << graph.get_edges().size() << ", {";
  for (const auto& color : colors_of_edges) {
    log_string << GraphPrinter::print_edge_color(color) << ": "
               << graph.get_colored_edges(color).size();
    if (color != colors_of_edges.back())
      log_string << ", ";
  }

  log_string << "}}\n}\n";

  return log_string.str();
}

std::string GraphPrinter::print_vertex(const Graph::Vertex& vertex) const {
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

std::string GraphPrinter::print_edge(const Graph::Edge& edge) const {
  return "\t  {\n\t\t\"id\":" + std::to_string(edge.id) +
         ",\n\t\t\"vertex_ids\":[" + std::to_string(edge.vertex_start) + "," +
         std::to_string(edge.vertex_end) + "],\n\t\t\"color\": \"" +
         print_edge_color(edge.color) + "\"\n\t  }";
}

std::string GraphPrinter::print_edge_color(const Color& color) {
  switch (color) {
    case Color::Grey:
      return "gray";
    case Color::Green:
      return "green";
    case Color::Yellow:
      return "yellow";
    case Color::Red:
      return "red";
  }
}

std::string GraphPrinter::print() const {
  std::string graph_string = "{";

  // write graph depth
  graph_string +=
      "\n\t\"depth\": " + std::to_string(graph_.get_depth_map().size() - 1) +
      ",";

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
}  // namespace uni_cource_cpp
