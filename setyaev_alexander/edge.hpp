

#pragma once

#include "vertex.hpp"

namespace uni_course_cpp {
using EdgeId = int;
struct Edge {
 public:
  enum class Color { Red, Grey, Green, Yellow };
  Edge(EdgeId id,
       VertexId first_vertex_id,
       VertexId second_vertex_id,
       const Edge::Color& color)
      : id_(id),
        first_vertex_id_(first_vertex_id),
        second_vertex_id_(second_vertex_id),
        color_(color) {}
  EdgeId get_id() const;
  VertexId get_first_vertex_id() const;
  VertexId get_second_vertex_id() const;
  Edge::Color get_color() const;

 private:
  EdgeId id_ = 0;
  VertexId first_vertex_id_ = 0;
  VertexId second_vertex_id_ = 0;
  Edge::Color color_ = Edge::Color::Grey;
};
}
