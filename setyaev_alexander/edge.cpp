#include "edge.hpp"

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

std::string Edge::to_string() const {
  std::stringstream json;
  json << "{\n  \"id\": " << get_id() << ",\n  \"vertex_ids\": ["
       << get_first_vertex_id() << ", " << get_second_vertex_id()
       << "],\n  \"color\": " << color_to_string(color_) << "\n}";
  return json.str();
}

std::string color_to_string(const Edge::Color& color) {
  if (color == Edge::Color::Grey) {
    return "\"grey\"";
  }
  if (color == Edge::Color::Yellow) {
    return "\"yellow\"";
  }
  if (color == Edge::Color::Green) {
    return "\"green\"";
  }
  if (color == Edge::Color::Red) {
    return "\"red\"";
  }
  throw std::runtime_error("Failed to determine color");
}
