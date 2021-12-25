#include "graph_printer.hpp"
#include <array>
#include <sstream>
#include <string>

namespace uni_course_cpp {

std::string GraphPrinter::edge_color_to_string(const Edge::Color& color) {
  switch (color) {
    case Edge::Color::Gray:
      return "gray";
    case Edge::Color::Green:
      return "green";
    case Edge::Color::Red:
      return "red";
    case Edge::Color::Yellow:
      return "yellow";
  }
  throw std::runtime_error("Color doesn't exist");
}

std::string GraphPrinter::print() const {
  std::stringstream log_string;
  log_string << "  depth: " << graph_.get_depth() << ",\n";
  log_string << "  vertices: " << graph_.get_vertices().size() << ", [";

  for (Depth depth = 0; depth < graph_.get_depth() - 1; depth++)
    log_string << graph_.get_vertex_ids_at_depth(depth).size() << ", ";
  log_string << graph_.get_vertex_ids_at_depth(graph_.get_depth() - 1).size()
             << "],\n"
             << "  edges: " << graph_.get_edges().size() << ", {";

  const auto colors =
      std::array<Edge::Color, 4>{Edge::Color::Gray, Edge::Color::Green,
                                 Edge::Color::Yellow, Edge::Color::Red};
  for (const auto& color : colors) {
    log_string << edge_color_to_string(color) << ": "
               << graph_.get_colored_edges(color).size();
    if (&color != colors.end() - 1) {
      log_string << ", ";
    }
  }
  log_string << "}\n}\n";
  return log_string.str();
}
}  // namespace uni_course_cpp
