#include "graph.hpp"
#include <string>

std::string get_vertex_string(const Vertex& vertex) {
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
std::string get_edge_string(const Edge& edge) {
  std::string str_edge = "\t  {\n\t\t\"id\":" + std::to_string(edge.id) +
                         ",\n\t\t\"vertex_ids\":[" +
                         std::to_string(edge.vertex_start) + "," +
                         std::to_string(edge.vertex_end) + "]\n\t  }";
  return str_edge;
}
std::string get_graph_string(const Graph& graph) {
  std::string str_graph = "{";

  // write graph depth
  str_graph +=
      "\n\t\"depth\": " + std::to_string(graph.get_graph_depth()) + ",";

  // write vertices
  str_graph += "\n\t\"vertices\": [\n";
  for (const auto& vertex : graph.get_vertices()) {
    str_graph += get_vertex_string(vertex);
    if (vertex.id != graph.get_vertices().back().id)
      str_graph += ",\n";
  }
  str_graph += "\n\t],";

  // write edges
  str_graph += "\n\t\"edges\": [\n";
  for (const auto& edge : graph.get_edges()) {
    str_graph += get_edge_string(edge);
    if (edge.id != graph.get_edges().back().id)
      str_graph += ",\n";
  }
  str_graph += "\n\t]";

  str_graph += "\n}\n";
  return str_graph;
}
