#include "graphprinter.hpp"
#include "graph.hpp"

std::string GraphPrinter::print() const {
  std::string result("{\"vertices\":[");
  result += print_vertices();
  result += "],\"edges\":[";
  result += print_edges();
  result += "]}";

  return result;
}

std::string GraphPrinter::print_vertices() const {
  std::string result;
  for (const Vertex& vertex : graph_.vertices()) {
    result += "{\"id\":";
    result += std::to_string(vertex.id);
    result += ",\"edge_ids\":[";
    for (const Edge& edge : graph_.edges()) {
      if ((vertex.id == edge.from_vertex_id) ||
          (vertex.id == edge.to_vertex_id)) {
        result += std::to_string(edge.id) + ",";
      }
    }
    result.pop_back();
    result += "]},";
  }
  result.pop_back();
  return result;
}

std::string GraphPrinter::print_edges() const {
  std::string result;
  for (const Edge& edge : graph_.edges()) {
    result += "{\"id\":";
    result += std::to_string(edge.id);
    result += ",\"vertex_ids\":[";
    result += std::to_string(edge.from_vertex_id);
    result += ",";
    result += std::to_string(edge.to_vertex_id);
    result += "]},";
  }
  result.pop_back();
  return result;
}
