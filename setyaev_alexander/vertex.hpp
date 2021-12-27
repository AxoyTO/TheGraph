

#pragma once

#include <vector>
namespace uni_course_cpp {
using VertexId = int;

class Vertex {
 public:
  explicit Vertex(VertexId id) : id_(id) {}
  VertexId get_id() const;

 private:
  VertexId id_ = 0;
};
}  // namespace uni_course_cpp
