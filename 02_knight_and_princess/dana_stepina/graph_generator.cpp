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

void generate_grey_edges(Graph& graph, const GraphGenerator::Params& params) {
  graph.add_vertex();  // Zero vertex

  for (Depth depth = 0; depth <= params.depth; depth++) {
    const auto& probability = get_probability(params.depth, depth);
    /*Required constant reference, since the vector being iterated over changes
      in the process*/
    const auto& vertex_ids = graph.get_vertex_ids_at(depth);

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
  /*Required constant reference, since the vector being iterated over changes
  in the process*/
  const auto& depth_map = graph.get_depth_map();

  for (const auto& vertex_ids : depth_map)
    for (const auto& vertex_id : vertex_ids)
      if (is_lucky(GREEN_PROBABILITY))
        graph.add_edge(vertex_id, vertex_id);
}

void generate_yellow_edges(Graph& graph) {
  /*Required constant reference, since the vector being iterated over changes
  in the process*/
  const auto& depth_map = graph.get_depth_map();

  for (Depth depth = 0; depth < depth_map.size() - 1; depth++) {
    const auto& probability =
        MAX_PROBABILITY - get_probability(depth_map.size() - 1, depth);
    const auto& from_vertex_ids = graph.get_vertex_ids_at(depth);
    const auto& to_vertex_ids = graph.get_vertex_ids_at(depth + 1);

    for (const auto& from_vertex_id : from_vertex_ids)
      for (const auto& to_vertex_id : to_vertex_ids)
        if (!graph.is_connected(from_vertex_id, to_vertex_id))
          if (is_lucky(probability))
            graph.add_edge(from_vertex_id, to_vertex_id);
  }
}

void generate_red_edges(Graph& graph) {
  /*Required constant reference, since the vector being iterated over changes
  in the process*/
  const auto& depth_map = graph.get_depth_map();

  for (Depth depth = 0; depth < depth_map.size() - 2; depth++)
    for (const auto& from_vertex_id : depth_map[depth])
      for (const auto& to_vertex_id : depth_map[depth + 2])
        if (is_lucky(RED_PROBABILITY))
          graph.add_edge(from_vertex_id, to_vertex_id);
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
