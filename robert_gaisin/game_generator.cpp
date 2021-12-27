#include "game_generator.hpp"
#include "game.hpp"
#include "graph.hpp"
#include "graph_generator.hpp"

#include <random>
namespace {
using uni_course_cpp::VertexId;
const VertexId& determine_position(const std::vector<VertexId>& set_vertices) {
  std::random_device rd;
  std::mt19937 mersenne(rd());
  return set_vertices[mersenne() % set_vertices.size()];
}
}  // namespace
namespace uni_course_cpp {
using uni_course_cpp::Game;
using uni_course_cpp::GraphGenerator;
using uni_course_cpp::VertexId;

Game GameGenerator::generate() const {
  const auto graph_generator = GraphGenerator(params_);
  const auto graph = graph_generator.generate();
  VertexId knight_position = 0;

  const auto& set_vertices_princess = graph.depth_map().back();

  VertexId princess_position = determine_position(set_vertices_princess);
  return Game(graph, knight_position, princess_position);
}

}  // namespace uni_course_cpp
