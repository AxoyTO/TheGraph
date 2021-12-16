#include "graph_printing.hpp"

namespace {
constexpr int COLORS_NUM = 4;
}  // namespace

namespace graph_printing {
std::string color_to_string(const Edge::Color& color) {
  switch (color) {
    case Edge::Color::Gray:
      return "gray";
    case Edge::Color::Green:
      return "green";
    case Edge::Color::Yellow:
      return "yellow";
    case Edge::Color::Red:
      return "red";
  }
}

std::string vertices_distribution(const Graph& graph) {
  std::string result_string;
  for (int depth = 0; depth < graph.get_depth(); depth++) {
    if (depth != 0)
      result_string += ", ";
    result_string +=
        std::to_string(graph.get_vertex_ids_in_depth(depth).size());
  }
  return result_string;
}

std::string edges_distribution(const Graph& graph) {
  std::string result_string;
  const std::array<Edge::Color, COLORS_NUM> colors = {
      Edge::Color::Gray, Edge::Color::Green, Edge::Color::Yellow,
      Edge::Color::Red};
  for (const auto& color : colors) {
    result_string += color_to_string(color);
    result_string += ": ";
    result_string += (std::to_string(graph.get_colored_edges(color).size()));
    if (color != Edge::Color::Red)
      result_string += ", ";
  }
  return result_string;
}

std::string graph_description(const Graph& graph) {
  return "\tdepth: " + std::to_string(graph.get_depth()) +
         ",\n\tvertices: {amount: " +
         std::to_string(graph.get_vertices().size()) + ", distribution: [" +
         graph_printing::vertices_distribution(graph) +
         "]},\n\tedges: {amount: " + std::to_string(graph.get_edges().size()) +
         ", distribution: {" + graph_printing::edges_distribution(graph) +
         "}}\n";
}

std::string graph_to_json_string(const Graph& graph) {
  std::string result_string;

  result_string = "{\n\t\"depth\": " + std::to_string(graph.get_depth()) +
                  ",\n\t\"vertices\": [\n";

  const auto& vertices = graph.get_vertices();
  for (int i = 0; i < vertices.size(); i++) {
    result_string += vertex_to_json_string(vertices[i]);
    if (i != vertices.size() - 1)
      result_string += ",";
    result_string += "\n";
  }
  result_string += "\t],\n";

  const auto& edges = graph.get_edges();
  result_string += "\t\"edges\": [\n";
  for (int i = 0; i < edges.size(); i++) {
    result_string += edge_to_json_string(edges[i]);
    if (i != edges.size() - 1)
      result_string += ",";
    result_string += "\n";
  }
  result_string += "\t]\n}\n";

  return result_string;
}

std::string edge_to_json_string(const Edge& edge) {
  return "\t\t{ \"id\": " + std::to_string(edge.id) + ", \"vertex_ids\": [" +
         std::to_string(edge.vertex_id1) + ", " +
         std::to_string(edge.vertex_id2) + "], \"color\": \"" +
         color_to_string(edge.color) + "\" }";
}

std::string vertex_to_json_string(const Vertex& vertex) {
  std::string result_string =
      "\t\t{ \"id\": " + std::to_string(vertex.id) + ", \"edge_ids\": [";

  const auto& edge_ids = vertex.get_edge_ids();
  for (int i = 0; i < edge_ids.size(); i++) {
    result_string += std::to_string(edge_ids[i]);
    if (i != edge_ids.size() - 1)
      result_string += ", ";
    else
      result_string += "], \"depth\": ";
  }
  result_string += std::to_string(vertex.depth);
  result_string += " }";
  return result_string;
}
}  // namespace graph_printing
