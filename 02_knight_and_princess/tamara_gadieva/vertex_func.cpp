#include "graph_generate.hpp"

void Vertex::add_edge_id(const EdgeId& id) {
  this->edge_ids.push_back(id);
}

std::string Vertex::json_string() const {
  std::string result_vertex_inf =
      "\t\t{ \"id\": " + std::to_string(id) + ", \"edge_ids\": [";

  for (int i = 0; i < edge_ids.size(); i++) {
    result_vertex_inf += std::to_string(edge_ids[i]);
    if (i != edge_ids.size() - 1)
      result_vertex_inf += ", ";
    else
      result_vertex_inf += "] }";
  }

  return result_vertex_inf;
}
