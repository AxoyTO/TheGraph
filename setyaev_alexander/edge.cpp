#include "edge.hpp"
namespace uni_course_cpp {
Edge::Color Edge::get_color() const {
  return color_;
}

EdgeId Edge::get_id() const {
  return id_;
}

VertexId Edge::get_first_vertex_id() const {
  return first_vertex_id_;
}

VertexId Edge::get_second_vertex_id() const {
  return second_vertex_id_;
}
}
