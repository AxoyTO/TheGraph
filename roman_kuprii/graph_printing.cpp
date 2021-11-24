#include <array>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

#include "graph.hpp"
#include "graph_printing.hpp"

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

std::string edge_to_json(const Edge& edge) {
  std::string res;
  res = "{ \"id\": ";
  res += to_string(edge.id);
  res += ", \"vertex_ids\": [";
  res += to_string(edge.connected_vertices[0]);
  res += ", ";
  res += to_string(edge.connected_vertices[1]);
  res += "], \"color\": ";
  res += color_to_string(edge.color);
  res += " }";
  return res;
}

std::string vertex_to_json(const Vertex& vertex) {
  std::string res;
  res = "{ \"id\": ";
  res += to_string(vertex.get_id()) + ", \"edge_ids\": [";
  for (const auto& edge_id : vertex.get_edges_ids()) {
    res += to_string(edge_id);
    res += ", ";
  }
  if (vertex.get_edges_ids().size() > 0) {
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
  for (const auto& vertex : graph.get_vertices()) {
    res += vertex_to_json(vertex);
    res += ", ";
  }
  if (graph.get_vertices().size()) {
    res.pop_back();
    res.pop_back();
  }
  res += " ], \"edges\": [ ";
  for (const auto& edge : graph.get_edges()) {
    res += edge_to_json(edge);
    res += ", ";
  }
  if (graph.get_edges().size() > 0) {
    res.pop_back();
    res.pop_back();
  }
  res += " ] }\n";
  return res;
}

}  // namespace graph_printing

}  // namespace uni_cpp_practice
