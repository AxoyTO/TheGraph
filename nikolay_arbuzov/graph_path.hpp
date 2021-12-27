#pragma once

#include <vector>
#include "graph.hpp"
namespace uni_course_cpp {
struct GraphPath {
  using Distance = int;
  GraphPath(std::vector<Graph::VertexId> _vertex_ids, Distance _distance)
      : vertex_ids(_vertex_ids), distance(_distance) {}
  std::vector<Graph::VertexId> vertex_ids;
  Distance distance = 0;
};

}  // namespace uni_course_cpp
