#pragma once

#include <vector>

#include "graph.hpp"

namespace uni_cource_cpp {
struct GraphPath {
 public:
  using Distance = int;

  GraphPath(std::vector<VertexId> vertex_ids, std::vector<EdgeId> edge_ids);

  Distance distance() const;

  std::vector<VertexId> vertex_ids;
  std::vector<EdgeId> edge_ids;
};
}  // namespace uni_cource_cpp
