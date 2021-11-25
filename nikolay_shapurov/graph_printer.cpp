#include "graph_printer.hpp"

GraphPrinter::GraphPrinter(const Graph& graph) : graph_(graph) {}

std::string GraphPrinter::print() const {
  std::string vertices_string;
  for (const Vertex& vertex : graph_.vertices()) {
    vertices_string += print_vertex(vertex);
  }
  if (!vertices_string.empty())
    vertices_string.pop_back();

  std::string edges_string;
  for (const Edge& edge : graph_.edges()) {
    edges_string += print_edge(edge);
  }
  if (!edges_string.empty())
    edges_string.pop_back();

  return "{\"vertices\":[" + vertices_string + "],\"edges\":[" + edges_string +
         "]}\n";
}

std::string GraphPrinter::print_vertex(const Vertex& vertex) const {
  std::string result("{\"id\":");
  result += std::to_string(vertex.id);
  result += ",\"edge_ids\":[";

  std::string vertex_edges_json;
  for (const EdgeId& edgeId : graph_.get_edge_ids(vertex.id)) {
    vertex_edges_json += std::to_string(edgeId) + ",";
  }
  if (!vertex_edges_json.empty()) {
    vertex_edges_json.pop_back();
  }

  return result + vertex_edges_json + "]},";
}

std::string GraphPrinter::print_edge(const Edge& edge) const {
  std::string result;
  result += "{\"id\":";
  result += std::to_string(edge.id);
  result += ",\"vertex_ids\":[";
  result += std::to_string(edge.from_vertex_id);
  result += ",";
  result += std::to_string(edge.to_vertex_id);

  return result + "]},";
}
