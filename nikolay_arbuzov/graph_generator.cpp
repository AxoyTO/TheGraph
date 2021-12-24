#include "graph_generator.hpp"
#include <random>
#include <vector>
#include "graph.hpp"

namespace {

constexpr float kGreenProbability = 0.1;
constexpr float kRedProbability = 0.33;

}  // namespace

namespace uni_course_cpp {

bool GraphGenerator::can_generate_vertex(float probability) {
  std::random_device random_device;
  std::mt19937 generate(random_device());
  std::bernoulli_distribution distribution(probability);
  return distribution(generate);
}
Graph::VertexId GraphGenerator::get_random_vertex_id(
    const std::vector<Graph::VertexId>& vertex_ids) {
  std::random_device random_device;
  std::mt19937 generator(random_device());
  std::uniform_int_distribution<> distribution(0, vertex_ids.size() - 1);
  return vertex_ids[distribution(generator)];
}
void GraphGenerator::generate_grey_edges(Graph& graph) {
  const auto& depth = params_.depth();
  const auto& new_vertices_count = params_.new_vertices_count();
  for (Graph::Depth current_depth = 0; current_depth < depth; ++current_depth) {
    // Not const reference, because I need to change depth in "for"
    auto vertices_on_current_depth =
        graph.get_vertex_ids_on_depth(current_depth);
    for (const auto& vertex_id : vertices_on_current_depth) {
      for (int new_vertices_number = 0;
           new_vertices_number < new_vertices_count; ++new_vertices_number) {
        if (can_generate_vertex(float(depth - current_depth) / depth)) {
          const auto& new_vertex = graph.add_vertex();
          graph.add_edge(vertex_id, new_vertex.id);
        }
      }
    }
  }
}

void GraphGenerator::generate_green_edges(Graph& graph) {
  const auto& vertices = graph.vertices();
  for (const auto& vertex : vertices) {
    if (can_generate_vertex(kGreenProbability))
      graph.add_edge(vertex.id, vertex.id);
  }
}

void GraphGenerator::generate_yellow_edges(Graph& graph) {
  const auto& depth = params_.depth();
  for (Graph::Depth current_depth = 0; current_depth < depth - 1;
       ++current_depth) {
    for (const auto& vertex_id : graph.get_vertex_ids_on_depth(current_depth)) {
      const auto& to_vertex_ids =
          graph.get_vertex_ids_on_depth(current_depth + 1);
      auto to_vertex_ids_no_neighbors = std::vector<Graph::VertexId>();

      for (const auto& not_neighbor_vertex_id : to_vertex_ids) {
        if (!graph.is_connected(vertex_id, not_neighbor_vertex_id)) {
          to_vertex_ids_no_neighbors.push_back(not_neighbor_vertex_id);
        }
      }

      if (to_vertex_ids_no_neighbors.size() &&
          can_generate_vertex(
              float(1) - (float(depth - 1 - current_depth) / (depth - 1)))) {
        graph.add_edge(vertex_id,
                       get_random_vertex_id(to_vertex_ids_no_neighbors));
      }
    }
  }
}

void GraphGenerator::generate_red_edges(Graph& graph) {
  const auto& depth = params_.depth();
  for (auto& vertex : graph.vertices()) {
    if (depth - graph.get_vertex_depth(vertex.id) >= 2) {
      const auto& to_vertex_ids =
          graph.get_vertex_ids_on_depth(graph.get_vertex_depth(vertex.id) + 2);
      if (to_vertex_ids.size() && can_generate_vertex(kRedProbability)) {
        graph.add_edge(vertex.id, get_random_vertex_id(to_vertex_ids));
      }
    }
  }
}

const Graph GraphGenerator::generate() {
  auto graph = Graph();
  graph.add_vertex();
  generate_grey_edges(graph);
  generate_green_edges(graph);
  generate_yellow_edges(graph);
  generate_red_edges(graph);
  return graph;
}

}  // namespace uni_course_cpp
