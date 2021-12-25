#include <array>
#include <sstream>

#include "graph.hpp"
#include "graph_printer.hpp"

namespace uni_cource_cpp {
GraphPrinter::GraphPrinter(Graph& input_graph) : graph_(input_graph) {}

static std::string print_edge_color(const EdgeColor& color) {
  switch (color) {
    case EdgeColor::Gray:
      return "gray";
    case EdgeColor::Green:
      return "green";
    case EdgeColor::Blue:
      return "blue";
    case EdgeColor::Yellow:
      return "yellow";
    case EdgeColor::Red:
      return "red";
  }
}

std::string GraphPrinter::print() const {
  std::stringstream json_stringstream;
  json_stringstream << "{\"depth\":" << graph_.depth() << ",";
  json_stringstream << "\"vertices\":[";
  const auto& vertices = graph_.vertices();
  for (auto it = vertices.begin(); it != vertices.end(); ++it) {
    json_stringstream << print_vertex(it->second);
    if (std::next(it) != graph_.vertices().end()) {
      json_stringstream << ",";
    }
  }
  json_stringstream << "],\"edges\":[";
  for (auto it = graph_.edges().begin(); it != graph_.edges().end(); ++it) {
    json_stringstream << print_edge(it->second);
    if (std::next(it) != graph_.edges().end()) {
      json_stringstream << ",";
    }
  }
  json_stringstream << "]}";
  return json_stringstream.str();
}

std::string GraphPrinter::print_graph_description() const {
  std::stringstream graph_description_stringstream;
  graph_description_stringstream << "{" << std::endl;
  graph_description_stringstream << "\t"
                                 << "depth: " << graph_.depth() << ","
                                 << std::endl;
  graph_description_stringstream
      << "\t"
      << "vertices: {amount: " << graph_.vertices().size()
      << ", distribution: [";
  for (int depth = 0; depth <= graph_.depth(); ++depth) {
    graph_description_stringstream
        << graph_.get_vertices_at_depth(depth).size();
    if (depth != graph_.depth()) {
      graph_description_stringstream << ", ";
    }
  }
  graph_description_stringstream << "]}," << std::endl
                                 << "\t"
                                 << "edges: {amount: " << graph_.edges().size()
                                 << ", distribution: {";
  const std::array<EdgeColor, EDGE_COLORS_COUNT> edge_colors = {
      EdgeColor::Gray, EdgeColor::Green, EdgeColor::Blue, EdgeColor::Yellow,
      EdgeColor::Red};
  for (auto it = edge_colors.begin(); it != edge_colors.end(); ++it) {
    graph_description_stringstream << print_edge_color(*it) << ": "
                                   << graph_.edge_ids_with_color(*it).size();
    if (std::next(it) != edge_colors.end()) {
      graph_description_stringstream << ", ";
    }
  }
  graph_description_stringstream << "}}" << std::endl;
  graph_description_stringstream << "}" << std::endl;
  return graph_description_stringstream.str();
}

std::string GraphPrinter::print_vertex(const Vertex& vertex) const {
  std::stringstream json_stringstream;
  json_stringstream << "{\"id\":" << vertex.id << ","
                    << "\"edge_ids\":[";
  for (auto it = vertex.connected_edges().begin();
       it != vertex.connected_edges().end(); ++it) {
    json_stringstream << *it;
    if (std::next(it) != vertex.connected_edges().end()) {
      json_stringstream << ",";
    }
  }
  json_stringstream << "],"
                    << "\"depth\":" << vertex.depth << "}";
  return json_stringstream.str();
}

std::string GraphPrinter::print_edge(const Edge& Edge) const {
  std::stringstream json_stringstream;
  json_stringstream << "{\"id\":" << Edge.id << ","
                    << "\"vertex_ids\":[" << Edge.vertex1_id << ","
                    << Edge.vertex2_id << "],\"color\":\""
                    << print_edge_color(Edge.color) << "\"}";
  return json_stringstream.str();
}

std::string GraphPrinter::print_paths(std::vector<GraphPath> paths) {
  // TODO: this print
  std::stringstream graph_paths_stringstream;
  graph_paths_stringstream << std::endl;
  return graph_paths_stringstream.str();
}
}  // namespace uni_cource_cpp
