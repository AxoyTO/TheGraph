#pragma once
#include "vertex.hpp"

struct Edge {
 public:
  enum class Color { Red = 0, Gray = 1, Green = 2, Yellow = 4 };
  Edge(EdgeId id,
       VertexId first_vertex_id,
       VertexId second_vertex_id,
       Edge::Color color)
      : id_(id),
        first_vertex_id_(first_vertex_id),
        second_vertex_id_(second_vertex_id),
        color_(color) {}
  EdgeId get_id() const;
  VertexId get_first_vertex_id() const;
  VertexId get_second_vertex_id() const;
  std::string to_string() const;
  Edge::Color get_color() const;

 private:
  EdgeId id_ = 0;
  VertexId first_vertex_id_ = 0;
  VertexId second_vertex_id_ = 0;
  Edge::Color color_ = Edge::Color::Gray;
};
