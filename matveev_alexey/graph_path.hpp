#pragma once

#include <vector>
#include "graph.hpp"

namespace uni_course_cpp {
struct GraphPath {
  using Distance = int;

  GraphPath(std::vector<VertexId> _vertex_ids, std::vector<EdgeId> _edge_ids)
      : vertex_ids(std::move(_vertex_ids)), edge_ids(std::move(_edge_ids)) {}

  GraphPath() {}

  Distance distance() const;

  std::vector<VertexId> vertex_ids;
  std::vector<EdgeId> edge_ids;
};
}  // namespace uni_course_cpp
