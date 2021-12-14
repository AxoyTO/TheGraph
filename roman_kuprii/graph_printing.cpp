#include <array>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

#include "graph.hpp"
#include "graph_printing.hpp"
#include "graph_traverser.hpp"

namespace {

using std::to_string;
using std::vector;

}  // namespace

namespace uni_cpp_practice {

namespace graph_printing {

std::string color_to_string(const Edge::Color& color) {
  switch (color) {
    case Edge::Color::Gray:
      return "\"gray\"";
    case Edge::Color::Green:
      return "\"green\"";
    case Edge::Color::Blue:
      return "\"blue\"";
    case Edge::Color::Yellow:
      return "\"yellow\"";
    case Edge::Color::Red:
      return "\"red\"";
  }
}

std::string edge_to_json(const std::pair<EdgeId, Edge>& edge) {
  std::string res;
  res = "{ \"id\": ";
  res += to_string(edge.first);
  res += ", \"vertex_ids\": [";
  res += to_string(edge.second.connected_vertices[0]);
  res += ", ";
  res += to_string(edge.second.connected_vertices[1]);
  res += "], \"color\": ";
  res += color_to_string(edge.second.color);
  res += " }";
  return res;
}

std::string vertex_to_json(const std::pair<VertexId, Vertex>& vertex) {
  std::string res;
  res = "{ \"id\": ";
  res += to_string(vertex.first) + ", \"edge_ids\": [";
  for (const auto& edge_id : vertex.second.get_edges_ids()) {
    res += to_string(edge_id);
    res += ", ";
  }
  if (vertex.second.get_edges_ids().size() > 0) {
    res.pop_back();
    res.pop_back();
  }
  res += "] }";
  return res;
}

std::string graph_to_json(const Graph& graph) {
  std::string res;
  res = "{ \"depth\": ";
  res += to_string(graph.get_depth());
  res += ", \"vertices\": [ ";
  for (const auto& vertex : graph.get_vertices_map()) {
    res += vertex_to_json(vertex);
    res += ", ";
  }
  if (graph.get_vertices_map().size()) {
    res.pop_back();
    res.pop_back();
  }
  res += " ], \"edges\": [ ";
  for (const auto& edge : graph.get_edges_map()) {
    res += edge_to_json(edge);
    res += ", ";
  }
  if (graph.get_edges_map().size() > 0) {
    res.pop_back();
    res.pop_back();
  }
  res += " ] }\n";
  return res;
}

std::string path_to_json(const GraphTraverser::Path& path) {
  std::string res;
  res = "{vertices: [";
  for (const auto& vertex_id : path.vertex_ids) {
    res += to_string(vertex_id);
    res += ", ";
  }
  if (path.vertex_ids.size()) {
    res.pop_back();
    res.pop_back();
  }
  res += "], distance: ";
  res += to_string(path.distance);
  res += "}";
  return res;
}

}  // namespace graph_printing

}  // namespace uni_cpp_practice
