#include "graph_json_printing.hpp"

namespace printing {
namespace json {

std::string graph_to_string(const Graph& graph) {
  std::stringstream json;

  json << "{\n\"vertices\": [\n";

  for (const auto& vertex : graph.get_vertices()) {
    json << vertex_to_string(vertex, graph.get_vertex_depth(vertex.get_id()),
                             graph.get_edges(vertex.get_id()));
    if (vertex.get_id() != graph.get_vertices().back().get_id()) {
      json << ",\n";
    }
  }

  json << "\n],\n\"edges\": [\n";

  for (const auto& edge : graph.get_edges()) {
    json << edge_to_string(edge);
    if (edge.get_id() != graph.get_edges().back().get_id()) {
      json << ",\n";
    }
  }

  json << "\n]\n}\n";

  return json.str();
}

std::string vertex_to_string(const Vertex& vertex,
                             Graph::Depth depth,
                             const std::vector<EdgeId>& connected_edge_ids) {
  std::stringstream json;
  json << "{\n  \"id\": " << vertex.get_id() << ",\n  \"edge_ids\": [";

  for (const auto edge_id : connected_edge_ids) {
    json << edge_id;
    if (edge_id != connected_edge_ids.back()) {
      json << ", ";
    }
  }

  json << "],\n  \"depth\": " << depth << "\n}";

  return json.str();
}

std::string edge_to_string(const Edge& edge) {
  std::stringstream json;
  json << "{\n  \"id\": " << edge.get_id() << ",\n  \"vertex_ids\": ["
       << edge.get_first_vertex_id() << ", " << edge.get_second_vertex_id()
       << "],\n  \"color\": " << color_to_string(edge.get_color()) << "\n}";
  return json.str();
}

}  // namespace json
}  // namespace printing
