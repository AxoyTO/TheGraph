#pragma once

#include "graph.hpp"

namespace uni_cource_cpp {

struct GraphPath {
  using Distance = int;

  GraphPath(const std::vector<Graph::VertexId>& _vertex_ids)
      : vertex_ids(_vertex_ids){};

  Distance distance() const { return vertex_ids.size() - 1; };

  std::vector<Graph::VertexId> vertex_ids;
};

}  // namespace uni_cource_cpp
