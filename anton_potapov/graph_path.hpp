#pragma once

#include <vector>

#include "graph.hpp"

namespace uni_cource_cpp {
class GraphPath {
 public:
  using Distance = int;

  GraphPath(std::vector<VertexId> vertex_ids, std::vector<EdgeId> edge_ids);

  const std::vector<VertexId>& path_vector_ids() const;

  Distance distance() const;

 private:
  std::vector<VertexId> vertex_ids_;
  std::vector<EdgeId> edge_ids_;
};
}  // namespace uni_cource_cpp
