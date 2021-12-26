#include "edge.hpp"

std::string color_to_string(const Edge::Color& color) {
  switch (color) {
    case Edge::Color::Grey:
      return "\"grey\"";
      break;
    case Edge::Color::Yellow:
      return "\"yellow\"";
      break;
    case Edge::Color::Green:
      return "\"green\"";
      break;
    case Edge::Color::Red:
      return "\"red\"";
      break;
  }

  throw std::runtime_error("Failed to determine color");
}

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
