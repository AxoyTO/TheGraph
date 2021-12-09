#include "graph_generator.hpp"

#include <iostream>
#include <random>

namespace {
constexpr float GREEN_PROB = 0.1;
constexpr float BLUE_PROB = 0.25;
constexpr float RED_PROB = 0.33;

bool random_bool(float true_prob) {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::bernoulli_distribution d(true_prob);
  return d(gen);
}

uni_cpp_practice::VertexId random_vertex_id(
    const std::vector<uni_cpp_practice::VertexId>& vertex_ids) {
  std::random_device dev;
  std::mt19937 rng(dev());
  std::uniform_int_distribution<std::mt19937::result_type> random_index(
      0, vertex_ids.size() - 1);
  return vertex_ids[random_index(rng)];
}

}  // namespace

namespace uni_cpp_practice {
Graph GraphGenerator::generate_random_graph() const {
  Graph graph = Graph();
  graph.add_new_vertex();

  generate_grey_edges(graph, params_.depth, params_.new_vertices_num);
  generate_green_edges(graph);
  generate_blue_edges(graph);
  generate_yellow_edges(graph);
  generate_red_edges(graph);

  return graph;
}

void GraphGenerator::generate_grey_edges(Graph& graph,
                                         int depth,
                                         int new_vertices_num) const {
  const float probability_decreasement = 1.0 / depth;
  float new_vertex_prob = 1.0;
  for (int cur_depth = 0; cur_depth < depth; cur_depth++) {
    if (graph.depths_map_.size() - 1 < cur_depth) {
      return;
    }
    const auto& vertex_ids_at_depth = graph.depths_map_[cur_depth];
    for (const VertexId cur_vertex_id : vertex_ids_at_depth) {
      for (int k = 0; k < new_vertices_num; k++) {
        if (random_bool(new_vertex_prob)) {
          const auto new_vertex_id = graph.add_new_vertex();
          graph.bind_vertices(cur_vertex_id, new_vertex_id);
        }
      }
    }
    new_vertex_prob -= probability_decreasement;
  }
}

void GraphGenerator::generate_green_edges(Graph& graph) const {
  for (const auto& cur_vertex : graph.get_vertices()) {
    if (random_bool(GREEN_PROB)) {
      graph.bind_vertices(cur_vertex.id, cur_vertex.id);
    }
  }
}

void GraphGenerator::generate_blue_edges(Graph& graph) const {
  for (int cur_depth = 0; cur_depth < graph.depths_map_.size(); cur_depth++) {
    const auto& vertex_ids_at_depth = graph.depths_map_[cur_depth];
    const int last_id = vertex_ids_at_depth[vertex_ids_at_depth.size() - 1];
    for (const VertexId cur_id : vertex_ids_at_depth) {
      if (cur_id != last_id && random_bool(BLUE_PROB)) {
        graph.bind_vertices(cur_id, cur_id + 1);
      }
    }
  }
}

void GraphGenerator::generate_yellow_edges(Graph& graph) const {
  float yellow_probability = 0;
  const float probability_increasement = 1.0 / (graph.depths_map_.size() - 1);
  for (int cur_depth = 0; cur_depth < graph.depths_map_.size() - 1;
       cur_depth++) {
    const auto& vertex_ids_at_depth = graph.depths_map_[cur_depth];
    const auto& vertex_ids_at_next_depth = graph.depths_map_[cur_depth + 1];
    for (const VertexId cur_id : vertex_ids_at_depth) {
      if (random_bool(yellow_probability)) {
        std::vector<VertexId> possible_connections;
        for (const VertexId next_id : vertex_ids_at_next_depth) {
          if (!graph.are_vertices_connected(cur_id, next_id)) {
            possible_connections.push_back(next_id);
          }
        }
        if (possible_connections.size() > 0) {
          const VertexId binding_id = random_vertex_id(possible_connections);
          graph.bind_vertices(cur_id, binding_id);
        }
      }
    }
    yellow_probability += probability_increasement;
  }
}

void GraphGenerator::generate_red_edges(Graph& graph) const {
  for (int cur_depth = 0; cur_depth < graph.depths_map_.size() - 2;
       cur_depth++) {
    const auto& vertex_ids_at_depth = graph.depths_map_[cur_depth];
    for (const VertexId cur_id : vertex_ids_at_depth) {
      if (random_bool(RED_PROB)) {
        const VertexId binding_id =
            random_vertex_id(graph.depths_map_[cur_depth + 2]);
        graph.bind_vertices(cur_id, binding_id);
      }
    }
  }
}
}  // namespace uni_cpp_practice
