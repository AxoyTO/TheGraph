#pragma once
#include "graph.hpp"

namespace uni_course_cpp {

using uni_course_cpp::Edge;
using uni_course_cpp::VertexId;

class GraphPath {
 public:
  using Distance = int;

  GraphPath(const std::vector<VertexId>& vertex_ids,
            const Edge::Duration path_duration)
      : vertex_ids_(vertex_ids), path_duration_(path_duration) {}

  Distance distance() const { return vertex_ids_.size() - 1; }

  const std::vector<VertexId>& get_vertex_ids() const { return vertex_ids_; }

  Edge::Duration duration() const { return path_duration_; }

 private:
  std::vector<VertexId> vertex_ids_;
  const Edge::Duration path_duration_;
};

}  // namespace uni_course_cpp
