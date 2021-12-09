#include "graph_generator.hpp"

#include <cmath>
#include <iostream>
#include <random>

using Probability = int;

constexpr int MAX_PROBABILITY = 100, GREEN_PROBABILITY = 10,
              RED_PROBABILITY = 33;

namespace {
Probability get_probability(Depth depth, Depth current_depth) {
  return floor(MAX_PROBABILITY * (depth - current_depth) / depth);
}

bool is_lucky(Probability probability) {
  std::random_device rand;
  std::mt19937 generator(rand());
  std::bernoulli_distribution distribution(probability * 0.01);
  return distribution(generator);
}

VertexId choose_random_vertex_id(const std::vector<VertexId>& vertex_ids) {
  std::random_device rand;
  std::default_random_engine generator(rand());
  std::uniform_int_distribution<int> distribution(0, vertex_ids.size() - 1);
  return vertex_ids[distribution(generator)];
}

void generate_grey_edges(Graph& graph, const GraphGenerator::Params& params) {
  graph.add_vertex();  // Zero vertex

  for (Depth depth = 0; depth <= params.depth; depth++) {
    const auto& probability = get_probability(params.depth, depth);
    /*A copy is needed, since the vector over which it is iterated changes in
     * the process*/
    const auto vertex_ids = graph.get_vertex_ids_at(depth);

    bool any_new_vertex_generated = false;

    for (const auto& vertex_id : vertex_ids)
      for (int i = 0; i < params.new_vertices_num; i++)
        if (is_lucky(probability)) {
          const auto& new_vertex = graph.add_vertex();
          graph.add_edge(vertex_id, new_vertex.id);
          any_new_vertex_generated = true;
        }

    if (!any_new_vertex_generated)
      break;
  }

  if (graph.get_depth_map().size() - 1 < params.depth)
    std::cout
        << "The resulting depth of the generated graph is less than expected.\n"
        << "Expected depth: " << params.depth
        << "\nThe resulting depth: " << graph.get_depth_map().size() - 1
        << std::endl;
}

void generate_green_edges(Graph& graph) {
  /*A copy is needed, since the vector over which it is iterated changes in the
   * process*/
  const auto depth_map = graph.get_depth_map();

  for (const auto& vertex_ids : depth_map)
    for (const auto& vertex_id : vertex_ids)
      if (is_lucky(GREEN_PROBABILITY))
        graph.add_edge(vertex_id, vertex_id);
}

std::vector<VertexId> get_unconnected_vertex_ids(
    const VertexId& from_vertex_id,
    const std::vector<VertexId>& vertex_ids,
    const Graph& graph) {
  std::vector<VertexId> unconnected_vertex_ids;
  for (const auto& vertex_id : vertex_ids)
    if (!graph.is_connected(from_vertex_id, vertex_id))
      unconnected_vertex_ids.push_back(vertex_id);
  return unconnected_vertex_ids;
}

void generate_yellow_edges(Graph& graph) {
  const auto& depth_map = graph.get_depth_map();

  for (Depth depth = 0; depth < depth_map.size() - 1; depth++) {
    const auto& probability =
        MAX_PROBABILITY - get_probability(depth_map.size() - 2, depth);

    for (const auto& from_vertex_id : depth_map[depth]) {
      if (is_lucky(probability)) {
        const std::vector<VertexId> vertex_ids = get_unconnected_vertex_ids(
            from_vertex_id, depth_map[depth + 1], graph);
        graph.add_edge(from_vertex_id, choose_random_vertex_id(vertex_ids));
      }
    }
  }
}

void generate_red_edges(Graph& graph) {
  const auto& depth_map = graph.get_depth_map();

  for (Depth depth = 0; depth < depth_map.size() - 2; depth++)
    for (const auto& from_vertex_id : depth_map[depth]) {
      if (is_lucky(RED_PROBABILITY)) {
        const std::vector<VertexId> vertex_ids = depth_map[depth + 2];
        graph.add_edge(from_vertex_id,
                       choose_random_vertex_id(depth_map[depth + 2]));
      }
    }
}
}  // namespace

Graph GraphGenerator::generate() const {
  auto graph = Graph();
  generate_grey_edges(graph, params_);
  generate_green_edges(graph);
  generate_yellow_edges(graph);
  generate_red_edges(graph);

  return graph;
}
