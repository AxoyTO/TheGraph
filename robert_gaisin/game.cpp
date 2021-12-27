#include "game.hpp"
#include "graph_path.hpp"
#include "graph_traverser.hpp"
namespace uni_course_cpp {

GraphPath Game::find_shortest_path() const {
  const auto traverser = GraphTraverser(map_);
  return traverser.find_shortest_path(knight_position_, princess_position_);
}

GraphPath Game::find_fastest_path() const {
  const auto traverser = GraphTraverser(map_);
  return traverser.find_shortest_path(knight_position_, princess_position_,
                                      true);
}
}  // namespace uni_course_cpp
