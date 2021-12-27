#pragma once
#include "graph.hpp"

namespace uni_course_cpp {

class GraphPath;

class Game {
 public:
  Game(const Graph& map,
       const VertexId& knight_position,
       const VertexId& princess_position)
      : map_(map),
        knight_position_(knight_position),
        princess_position_(princess_position){};
  // Traverse by `Distance`
  GraphPath find_shortest_path() const;
  // Traverse by `Duration`
  GraphPath find_fastest_path() const;
  const Graph& map() const { return map_; }
  const VertexId& knight_position() const { return knight_position_; }
  const VertexId& princess_position() const { return princess_position_; }

 private:
  Graph map_;
  VertexId knight_position_;
  VertexId princess_position_;
};
}  // namespace uni_course_cpp
