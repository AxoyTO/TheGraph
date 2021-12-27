#include "graph_generator.hpp"
#include <random>

namespace {
constexpr double kGreenProbability = 0.1;
constexpr double kRedProbability = 0.33;

bool check_probability(double probability) {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::bernoulli_distribution d(probability);
  return d(gen);
}

int get_random_index(int size) {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> distrib(0, size);
  return distrib(gen);
}
}  // namespace
namespace uni_course_cpp {

void GraphGenerator::generate_gray_edges(Graph& graph) const {
  for (int depth = 0; depth < params_.depth(); ++depth) {
    // const for not to iterate changing objects
    const std::vector<VertexId> vertices = graph.get_vertex_ids_at_depth(depth);
    for (const auto& vertex : vertices) {
      if (check_probability(1.0 - (double)depth / (double)params_.depth())) {
        for (int i = 0; i < params_.new_vertices_count(); ++i) {
          const auto& new_vertex = graph.add_vertex();
          graph.add_edge(vertex, new_vertex.get_id());
        }
      }
    }
  }
}

void GraphGenerator::generate_green_edges(Graph& graph) const {
  for (const auto& vertex : graph.get_vertices()) {
    if (check_probability(kGreenProbability)) {
      graph.add_edge(vertex.get_id(), vertex.get_id());
    }
  }
}

std::vector<VertexId> get_unconected_vertex_ids(
    const Graph& graph,
    const Vertex& vertex,
    const std::vector<VertexId>& vertices_at_depth) {
  std::vector<VertexId> vertices_ids;
  for (const auto& another_vertex : vertices_at_depth) {
    if (!(graph.is_connected(vertex.get_id(), another_vertex))) {
      vertices_ids.push_back(another_vertex);
    }
  }
  return vertices_ids;
}

void GraphGenerator::generate_yellow_edges(Graph& graph) const {
  for (const auto& first_vertex : graph.get_vertices()) {
    if (graph.get_vertex_depth(first_vertex.get_id()) == graph.get_depth()) {
      continue;
    }

    if (check_probability(
            (double)graph.get_vertex_depth(first_vertex.get_id()) /
            ((double)params_.depth() - 1.0))) {
      const std::vector<VertexId>& second_vertices_ids =
          graph.get_vertex_ids_at_depth(
              graph.get_vertex_depth(first_vertex.get_id()) + 1);
      const std::vector<VertexId> unconnected_vertex_ids =
          get_unconected_vertex_ids(graph, first_vertex, second_vertices_ids);

      if (unconnected_vertex_ids.size() > 0) {
        const VertexId second_vertex_id =
            unconnected_vertex_ids[get_random_index(
                unconnected_vertex_ids.size() - 1)];

        graph.add_edge(first_vertex.get_id(), second_vertex_id);
      }
    }
  }
}
void GraphGenerator::generate_red_edges(Graph& graph) const {
  for (const auto& first_vertex : graph.get_vertices()) {
    if (graph.get_vertex_depth(first_vertex.get_id()) ==
        graph.get_depth() - 1) {
      continue;
    }

    if (check_probability(kRedProbability)) {
      std::vector<VertexId> second_vertices_ids;
      for (const auto& second_vertex : graph.get_vertices()) {
        if (graph.get_vertex_depth(second_vertex.get_id()) ==
            graph.get_vertex_depth(first_vertex.get_id()) + 2) {
          second_vertices_ids.push_back(second_vertex.get_id());
        }
      }
      if (second_vertices_ids.size() > 0) {
        const VertexId second_vertex_id = second_vertices_ids[get_random_index(
            second_vertices_ids.size() - 1)];
        graph.add_edge(first_vertex.get_id(), second_vertex_id);
      }
    }
  }
}

Graph GraphGenerator::generate() const {
  auto graph = Graph();
  graph.add_vertex();

  generate_gray_edges(graph);

  generate_green_edges(graph);

  generate_yellow_edges(graph);

  generate_red_edges(graph);
  return graph;
}

}  // namespace uni_course_cpp
