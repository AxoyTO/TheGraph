#include "graph_printer.hpp"

std::string Graph_printer::add_vertex_string(const Vertex& vertex) {
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
std::string Graph_printer::add_edge_string(const Edge& edge) {
  return "\t  {\n\t\t\"id\":" + std::to_string(edge.id) +
         ",\n\t\t\"vertex_ids\":[" + std::to_string(edge.vertex_start) + "," +
         std::to_string(edge.vertex_end) + "]\n\t  }";
}
void Graph_printer::add_graph_string(const Graph& graph) {
  graph_string_ = "{";
  // write graph depth
  graph_string_ +=
      "\n\t\"depth\": " + std::to_string(graph.get_graph_depth()) + ",";
  // write vertices
  graph_string_ += "\n\t\"vertices\": [\n";
  for (const auto& vertex : graph.get_vertices()) {
    graph_string_ += add_vertex_string(vertex);
    if (vertex.id != graph.get_vertices().back().id)
      graph_string_ += ",\n";
  }
  graph_string_ += "\n\t],";

  // write edges
  graph_string_ += "\n\t\"edges\": [\n";
  for (const auto& edge : graph.get_edges()) {
    graph_string_ += add_edge_string(edge);
    if (edge.id != graph.get_edges().back().id)
      graph_string_ += ",\n";
  }
  graph_string_ += "\n\t]";

  graph_string_ += "\n}\n";
  //   std::cout<<graph_string_<<std::endl;
}
