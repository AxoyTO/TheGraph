#include "game_generator.hpp"
#include "game.hpp"
#include "graph.hpp"
#include "graph_generator.hpp"

#include <random>

namespace uni_course_cpp {
using uni_course_cpp::Game;
using uni_course_cpp::GraphGenerator;
using uni_course_cpp::VertexId;

Game GameGenerator::generate() const {
  const auto graph_generator = GraphGenerator(params_);
  const auto graph = graph_generator.generate();
  VertexId knight_position = 0;
  const auto set_vertices_princess = graph.depth_map().back();
  std::random_device rd;
  std::mt19937 mersenne(rd());
  VertexId princess_position =
      set_vertices_princess[mersenne() % set_vertices_princess.size()];
  return Game(graph, knight_position, princess_position);
}

}  // namespace uni_course_cpp
