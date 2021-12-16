#include <sstream>

#include "graph.hpp"
#include "graph_printer.hpp"

namespace uni_cource_cpp {
GraphPrinter::GraphPrinter(Graph& input_graph) : graph(input_graph) {}

std::string GraphPrinter::print() const {
  std::stringstream json_stringstream;
  json_stringstream << "{\"depth\":" << graph.max_depth() << ",";
  json_stringstream << "\"vertices\":[";
  const auto vertices = graph.vertices();
  for (auto it = vertices.begin(); it != vertices.end(); ++it) {
    json_stringstream << print_vertex(it->second);
    if (std::next(it) != graph.vertices().end()) {
      json_stringstream << ",";
    }
  }
  json_stringstream << "],\"edges\":[";
  for (auto it = graph.edges().begin(); it != graph.edges().end(); ++it) {
    json_stringstream << print_edge(it->second);
    if (std::next(it) != graph.edges().end()) {
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
                                 << "depth: " << graph.max_depth() << ","
                                 << std::endl;
  graph_description_stringstream
      << "\t"
      << "vertices: {amount: " << graph.vertices().size()
      << ", distribution: [";
  for (int depth = 0; depth <= graph.max_depth(); ++depth) {
    graph_description_stringstream << graph.get_vertices_at_depth(depth).size();
    if (depth != graph.max_depth()) {
      graph_description_stringstream << ", ";
    }
  }
  graph_description_stringstream << "]}," << std::endl;
  graph_description_stringstream
      << "\t"
      << "edges: {amount: " << graph.edges().size() << ", distribution: {"
      << "gray: " << graph.edge_with_color_cnt(uni_cource_cpp::EdgeColor::Gray)
      << ", green: "
      << graph.edge_with_color_cnt(uni_cource_cpp::EdgeColor::Green)
      << ", blue: "
      << graph.edge_with_color_cnt(uni_cource_cpp::EdgeColor::Blue)
      << ", yellow: "
      << graph.edge_with_color_cnt(uni_cource_cpp::EdgeColor::Yellow)
      << ", red: " << graph.edge_with_color_cnt(uni_cource_cpp::EdgeColor::Red);
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
