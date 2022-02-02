#pragma once
#include <vector>
#include "graph.hpp"

namespace uni_course_cpp {
class GraphPath {
 public:
  using Distance = int;

  GraphPath(const Distance& _distance, std::vector<VertexId> _vertex_ids)
      : vertex_ids(std::move(_vertex_ids)), distance(_distance) {}

  const std::vector<VertexId> vertex_ids;
  const Distance distance = 0;
};

}  // namespace uni_course_cpp
