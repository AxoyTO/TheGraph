#pragma once

#include "graph.hpp"

class GraphPrinter {
 public:
  explicit GraphPrinter(const Graph& input_graph) : graph(input_graph) {}

  std::string print() const {
    // doesn't work need to update vertices depth
    std::stringstream json_stringstream;
    json_stringstream << "{\"depth\":" << graph.max_depth() << ",";
    json_stringstream << "\"vertices\":[";
    for (auto it = graph.vertices().begin(); it != graph.vertices().end();
         ++it) {
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

 private:
  const Graph& graph;

  std::string print_vertex(const Vertex& vertex) const {
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

  std::string print_edge(const Edge& Edge) const {
    std::stringstream json_stringstream;
    std::string color_string = get_edge_color_string(Edge.color);
    json_stringstream << "{\"id\":" << Edge.id << ","
                      << "\"vertex_ids\":[" << Edge.vertex1_id << ","
                      << Edge.vertex2_id << "],\"color\":\"" << color_string
                      << "\"}";
    return json_stringstream.str();
  }
};
