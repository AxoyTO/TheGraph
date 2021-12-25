#pragma once

namespace uni_course_cpp {

struct GraphPath {
 public:
  using Distance = int;

  GraphPath(const std::vector<VertexId>& vertex_ids)
      : vertex_ids_(vertex_ids) {}

  Distance distance() const { return vertex_ids_.size() - 1; }

  const std::vector<VertexId>& get_vertex_ids() const { return vertex_ids_; }

 private:
  std::vector<VertexId> vertex_ids_;
};

}  // namespace uni_course_cpp
