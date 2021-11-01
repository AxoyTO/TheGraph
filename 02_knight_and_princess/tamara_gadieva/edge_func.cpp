#include "graph_generate.hpp"

std::string Edge::json_string() const {
  std::string result_edge_inf = "\t\t{ \"id\": " + std::to_string(this->id) +
                                ", \"vertex_ids\": [" +
                                std::to_string(this->vertex_id1) + ", " +
                                std::to_string(this->vertex_id2) + "] }";

  return result_edge_inf;
}