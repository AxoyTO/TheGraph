#include "graph_printer.hpp"

std::string GraphPrinter::edge_to_string(const Graph::Edge& edge) const {
  std::stringstream buffer;
  buffer << "{\"id\":" << edge.id << ",\"vertex_ids\":["
         << edge.vertex_ids.first << "," << edge.vertex_ids.second
         << "],\"color\":\"" << color_to_string(edge.color) << "\"}";
  return buffer.str();
}

std::string GraphPrinter::vertex_to_string(const Graph::Vertex& vertex) const {
  std::stringstream buffer;
  buffer << "{\"id\":" << vertex.id << ",\"edge_ids\":[";
  auto edge_ids = vertex.get_edge_ids();
  for (int i = 0; i < edge_ids.size() - 1; i++)
    buffer << edge_ids[i] << ",";
  buffer << edge_ids[edge_ids.size() - 1]
         << "],\"depth\":" << vertex.get_depth() << "}";
  return buffer.str();
}

std::string GraphPrinter::print() const {
  std::stringstream buffer;
  const vector<Graph::Vertex> vertices = graph_.get_vertices();
  const vector<Graph::Edge> edges = graph_.get_edges();
  buffer << "{\"depth\":" << graph_.get_depth() << ",\"vertices\":[";
  for (int j = 0; j < vertices.size(); j++) {
    Graph::Vertex vertex = vertices[j];
    buffer << vertex_to_string(vertex);
    if (j != vertices.size() - 1)
      buffer << ",";
  }
  buffer << "],\"edges\":[";
  for (int j = 0; j < edges.size(); j++) {
    Graph::Edge edge = edges[j];
    buffer << edge_to_string(edge);
    if (j != edges.size() - 1)
      buffer << ",";
  }
  buffer << "]}\n";
  return buffer.str();
}