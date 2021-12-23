#pragma once

#include <vector>
#include "graph.hpp"

namespace uni_cource_cpp {

struct GraphPath {
  using Distance = int;

  GraphPath(const std::vector<VertexId>& vertices_id,
            const std::vector<EdgeId>& edges_id)
      : vertex_ids(vertices_id), edge_ids(edges_id) {}

  Distance distance() const;

  std::vector<VertexId> vertex_ids;
  std::vector<EdgeId> edge_ids;
};

}  // namespace uni_cource_cpp
