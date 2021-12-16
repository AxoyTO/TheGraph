#include <sstream>

#include "graph.hpp"
#include "graph_printer.hpp"

namespace uni_cource_cpp {
GraphPrinter::GraphPrinter(Graph& input_graph) : graph_(input_graph) {}

std::string GraphPrinter::print() const {
  std::stringstream json_stringstream;
  json_stringstream << "{\"depth\":" << graph_.max_depth() << ",";
  json_stringstream << "\"vertices\":[";
  const auto vertices = graph_.vertices();
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
                                 << "depth: " << graph_.max_depth() << ","
                                 << std::endl;
  graph_description_stringstream
      << "\t"
      << "vertices: {amount: " << graph_.vertices().size()
      << ", distribution: [";
  for (int depth = 0; depth <= graph_.max_depth(); ++depth) {
    graph_description_stringstream
        << graph_.get_vertices_at_depth(depth).size();
    if (depth != graph_.max_depth()) {
      graph_description_stringstream << ", ";
    }
  }
  graph_description_stringstream << "]}," << std::endl;
  graph_description_stringstream
      << "\t"
      << "edges: {amount: " << graph_.edges().size() << ", distribution: {"
      << "gray: "
      << graph_.edge_ids_with_color(uni_cource_cpp::EdgeColor::Gray).size()
      << ", green: "
      << graph_.edge_ids_with_color(uni_cource_cpp::EdgeColor::Green).size()
      << ", blue: "
      << graph_.edge_ids_with_color(uni_cource_cpp::EdgeColor::Blue).size()
      << ", yellow: "
      << graph_.edge_ids_with_color(uni_cource_cpp::EdgeColor::Yellow).size()
      << ", red: "
      << graph_.edge_ids_with_color(uni_cource_cpp::EdgeColor::Red).size();
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
  std::string color_string = get_edge_color_string(Edge.color);
  json_stringstream << "{\"id\":" << Edge.id << ","
                    << "\"vertex_ids\":[" << Edge.vertex1_id << ","
                    << Edge.vertex2_id << "],\"color\":\"" << color_string
                    << "\"}";
  return json_stringstream.str();
}
}  // namespace uni_cource_cpp
