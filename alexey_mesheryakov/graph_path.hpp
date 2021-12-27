#pragma once
#include <vector>
#include "graph.hpp"

namespace uni_course_cpp {
struct GraphPath {
  using Distance = int;

  GraphPath(Distance _distance, std::vector<VertexId> _vertex_ids)
      : vertex_ids(_vertex_ids), distance(_distance) {}
  std::vector<VertexId> vertex_ids;
  Distance distance = 0;
};

}  // namespace uni_course_cpp