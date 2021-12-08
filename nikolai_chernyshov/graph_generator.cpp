#include "graph_generator.hpp"
#include <random>

namespace {
using VertexId = uni_course_cpp::VertexId;
using Graph = uni_course_cpp::Graph;

constexpr double GREEN_EDGE_PROBA = 0.1;
constexpr double RED_EDGE_PROBA = 0.33;

bool get_random_boolean(double proba) {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::bernoulli_distribution random_boolean(proba);
  return random_boolean(gen);
}

const VertexId& get_random_vertex_id(const std::vector<VertexId>& vertex_ids) {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> random_vertex(0, vertex_ids.size() - 1);
  return vertex_ids[random_vertex(gen)];
}
}  // namespace

namespace uni_course_cpp {
Graph GraphGenerator::generate() const {
  Graph graph;

  add_gray_edges(graph);
  add_green_edges(graph);
  add_yellow_edges(graph);
  add_red_edges(graph);

  return graph;
}

void GraphGenerator::add_gray_edges(Graph& graph) const {
  double proba_step = 1.0 / (double)params_.depth;

  graph.add_vertex();
  for (int depth = 0; depth < params_.depth; depth++) {
    // создал копию, чтобы не итерироваться по изменяемому массиву
    // т.к. при создании серых граней у новых вершин пересчитывается глубина
    const auto vertex_ids = graph.get_vertex_ids_in_depth(depth);

    for (const auto& vertex_id : vertex_ids) {
      for (int generate_try_num = 0;
           generate_try_num < params_.new_vertices_num; generate_try_num++) {
        if (get_random_boolean(1 - (double)depth * proba_step)) {
          const auto new_vertex_id = graph.add_vertex();
          graph.add_edge(vertex_id, new_vertex_id);
        }
      }
    }
  }
}

void GraphGenerator::add_green_edges(Graph& graph) const {
  for (const auto& vertex : graph.get_vertices())
    if (get_random_boolean(GREEN_EDGE_PROBA))
      graph.add_edge(vertex.id, vertex.id);
}

void GraphGenerator::add_yellow_edges(Graph& graph) const {
  // поставил 2 вместо 1, потому что с глубины 0 желтая грань не может
  // начинаться иначе она будет идти к потомку корневой вершины, что
  // противоречит условию работало бы и в случае 1, но тогда производились бы
  // лишние вычисления
  if (graph.get_depth() < 2)
    return;

  double proba_step = 1.0 / (double)(graph.get_depth() - 1);
  const auto& depth_map = graph.get_depth_map();

  for (auto vertex_ids_in_depth = depth_map.begin();
       vertex_ids_in_depth != depth_map.end() - 1; vertex_ids_in_depth++) {
    for (const auto& vertex_id : *vertex_ids_in_depth) {
      std::vector<VertexId> unconnected_vertex_ids;
      for (const auto& vertex_id_in_next_depth : *(vertex_ids_in_depth + 1)) {
        if (!graph.are_connected(vertex_id, vertex_id_in_next_depth)) {
          unconnected_vertex_ids.push_back(vertex_id_in_next_depth);
        }
      }
      if (unconnected_vertex_ids.size() &&
          get_random_boolean(
              proba_step * (double)(vertex_ids_in_depth - depth_map.begin()))) {
        graph.add_edge(vertex_id, get_random_vertex_id(unconnected_vertex_ids));
      }
    }
  }
};

void GraphGenerator::add_red_edges(Graph& graph) const {
  if (graph.get_depth() < 2)
    return;
  for (auto vertex_ids_in_depth = graph.get_depth_map().begin();
       vertex_ids_in_depth != graph.get_depth_map().end() - 2;
       vertex_ids_in_depth++)
    for (const auto& vertex_id : *vertex_ids_in_depth)
      if (get_random_boolean(RED_EDGE_PROBA))
        graph.add_edge(vertex_id,
                       get_random_vertex_id(*(vertex_ids_in_depth + 2)));
}
}  // namespace uni_course_cpp
