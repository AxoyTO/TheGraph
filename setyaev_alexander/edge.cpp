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
       << "],\n\"color\": ";
  if (color_ == Edge::Color::Gray) {
    json << "\"gray\"\n";
  }
  if (color_ == Edge::Color::Yellow) {
    json << "\"yellow\"\n";
  }
  if (color_ == Edge::Color::Green) {
    json << "\"green\"\n";
  }
  if (color_ == Edge::Color::Red) {
    json << "\"red\"\n";
  }
  json << "}";
  return json.str();
}
