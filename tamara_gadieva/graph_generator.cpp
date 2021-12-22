#include "graph_generator.hpp"
#include <random>
#include <stdexcept>
#include "graph.hpp"

namespace {
constexpr float GREEN_EDGE_PROB = 0.1;
constexpr float RED_EDGE_PROB = 0.33;

using Graph = uni_cource_cpp::Graph;

bool should_create_new_element(float probability) {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::bernoulli_distribution d(probability);
  return d(gen);
}

const VertexId& get_random_vertex_id(
    const std::vector<VertexId>& vertices_ids) {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<int> d(0, vertices_ids.size() - 1);
  return vertices_ids[d(gen)];
}

const std::vector<VertexId> get_unconnected_vertices(
    const VertexId& id,
    const std::vector<VertexId>& vertices_ids,
    const Graph& graph) {
  std::vector<VertexId> unconnected_vertices;
  for (const auto& vertex_id : vertices_ids)
    if (!graph.is_connected(id, vertex_id))
      unconnected_vertices.emplace_back(vertex_id);
  return unconnected_vertices;
}

void generate_gray_edges(Graph& graph, int max_depth, int new_vertices_num) {
  graph.add_vertex();
  for (int depth = 0; depth < max_depth - 1; depth++) {
    bool was_new_vertex_created = false;
    const float prob_of_creating_new_vertex =
        (float)(max_depth - depth) / (float)max_depth;
    for (const auto vertex_id_in_current_depth :
         graph.get_vertex_ids_in_depth(depth)) {
      for (int i = 0; i < new_vertices_num; i++)
        if (should_create_new_element(prob_of_creating_new_vertex)) {
          was_new_vertex_created = true;
          const auto& vertex_id_in_new_depth = graph.add_vertex();
          graph.add_edge(vertex_id_in_current_depth, vertex_id_in_new_depth);
        }
    }
    if (!was_new_vertex_created) {
      break;
    }
  }
}

void generate_green_edges(Graph& graph) {
  for (int depth = 0; depth < graph.get_depth(); depth++)
    for (const auto& vertex_id_in_current_depth :
         graph.get_vertex_ids_in_depth(depth))
      if (should_create_new_element(GREEN_EDGE_PROB))
        graph.add_edge(vertex_id_in_current_depth, vertex_id_in_current_depth);
}

void generate_yellow_edges(Graph& graph) {
  for (int depth = 0; depth < graph.get_depth() - 2; depth++) {
    const float prob_of_creating_new_edge =
        (float)depth / (float)(graph.get_depth() - 1);
    const auto& vertices_ids_in_current_depth =
        graph.get_vertex_ids_in_depth(depth);
    const auto& vertices_ids_in_prev_depth =
        graph.get_vertex_ids_in_depth(depth + 1);
    for (const auto& vertex_id_in_current_depth :
         vertices_ids_in_current_depth) {
      if (should_create_new_element(prob_of_creating_new_edge)) {
        auto unconnected_vertices = get_unconnected_vertices(
            vertex_id_in_current_depth, vertices_ids_in_prev_depth, graph);
        if (unconnected_vertices.size() == 0)
          continue;
        const auto& random_vertex_id_in_prev_depth =
            get_random_vertex_id(unconnected_vertices);
        graph.add_edge(vertex_id_in_current_depth,
                       random_vertex_id_in_prev_depth);
      }
    }
  }
}

void generate_red_edges(Graph& graph) {
  for (int depth = 0; depth < graph.get_depth() - 2; depth++) {
    const auto& vertices_ids_in_current_depth =
        graph.get_vertex_ids_in_depth(depth);
    const auto& vertices_ids_in_next_depth =
        graph.get_vertex_ids_in_depth(depth + 2);
    for (const auto& vertex_id_in_current_depth : vertices_ids_in_current_depth)
      if (should_create_new_element(RED_EDGE_PROB))
        graph.add_edge(vertex_id_in_current_depth,
                       get_random_vertex_id(vertices_ids_in_next_depth));
  }
}
}  // namespace

namespace uni_cource_cpp {
Graph GraphGenerator::generate() const {
  Graph graph;
  if (params_.depth == 0 || params_.new_vertices_num == 0)
    return graph;
  generate_gray_edges(graph, params_.depth, params_.new_vertices_num);
  generate_green_edges(graph);
  generate_yellow_edges(graph);
  generate_red_edges(graph);
  return graph;
}
}  // namespace uni_cource_cpp
