#include "vertex.hpp"

int Vertex::get_level() const {
  return level_;
}

VertexId Vertex::get_id() const {
  return id_;
}

const std::vector<EdgeId>& Vertex::get_connected_edge_ids() const {
  return connected_edge_ids_;
}

void Vertex::add_connected_edge_id(EdgeId id) {
  connected_edge_ids_.push_back(id);
}

std::string Vertex::to_string() const {
  std::stringstream json;
  json << "{\n  \"id\": " << get_id() << ",\n  \"edge_ids\": [";
  const auto& edge_ids = get_connected_edge_ids();
  for (const auto edge_id : edge_ids) {
    json << edge_id;
    if (edge_id != edge_ids.back()) {
      json << ", ";
    }
  }

  json << "],\n\"depth\": " << get_level() << "\n}";

  return json.str();
}
