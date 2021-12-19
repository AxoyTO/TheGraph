#include "graph_generator.hpp"
#include "graph.hpp"

#include <random>

namespace {

using uni_cource_cpp::EdgeColor;
using uni_cource_cpp::Graph;
using uni_cource_cpp::VertexId;

constexpr int PROBA_GRAY_BEGIN = 100;
constexpr int PROBA_RED = 33;
constexpr int PROBA_GREEN = 10;
constexpr int PROBA_BLUE = 25;

bool to_be_or_not_to_be(int proba) {
  std::random_device rd;
  std::mt19937 mersenne(rd());
  return (int)(mersenne() % 100 + 1) <= proba;
}

void generate_gray_edges(int graph_depth,  //наибольшая возможная глубина
                         int new_vertices_num,
                         Graph& graph) {
  for (int depth = 0; depth < graph_depth - 1; ++depth) {
    if (depth >= graph.depth() + 1) {
      break;
    }
    int proba_gray = PROBA_GRAY_BEGIN - depth * PROBA_GRAY_BEGIN / graph_depth;
    for (const auto& vertex_id : graph.depth_map()[depth]) {
      for (int c = 0; c < new_vertices_num; ++c) {
        if (to_be_or_not_to_be(proba_gray)) {
          const auto new_vertex_id = graph.add_vertex();
          graph.add_edge(vertex_id, new_vertex_id, EdgeColor::Gray);
        }
      }
    }
  }
}

void generate_green_edges(Graph& graph) {
  for (auto& vertex : graph.vertices()) {
    if (to_be_or_not_to_be(PROBA_GREEN)) {
      graph.add_edge(vertex.id, vertex.id, EdgeColor::Green);
    }
  }
}

void generate_blue_edges(Graph& graph) {
  for (const auto& vector_ids : graph.depth_map()) {
    for (auto vertex_id_iter = vector_ids.begin();
         vertex_id_iter < vector_ids.end() - 1; ++vertex_id_iter) {
      if (to_be_or_not_to_be(PROBA_BLUE)) {
        graph.add_edge(*vertex_id_iter, *(vertex_id_iter + 1), EdgeColor::Blue);
      }
    }
  }
}

std::vector<VertexId> get_unconnected_vertex_ids(
    const std::vector<VertexId>& layer,
    const VertexId& vert_id,
    const Graph& graph) {
  std::vector<VertexId> returned_vector;
  for (auto& vertex_id : layer) {
    if (graph.is_connected(vert_id, vertex_id))
      continue;
    returned_vector.push_back(vertex_id);
  }
  return returned_vector;
}

VertexId get_random_vertex_id(std::vector<VertexId> set_of_vertices_id) {
  std::random_device rd;
  std::mt19937 mersenne(rd());
  return set_of_vertices_id[mersenne() % set_of_vertices_id.size()];
}

void generate_yellow_edges(Graph& graph) {
  if (graph.depth() < 2)
    return;

  for (auto vertex_ids_at_depth = graph.depth_map().begin();
       vertex_ids_at_depth != graph.depth_map().end() - 1;
       ++vertex_ids_at_depth) {
    const int depth = graph.get_vertex((*vertex_ids_at_depth).front()).depth;
    const int proba_yellow = ((float)depth / (float)(graph.depth() - 1)) * 100;
    for (auto vertex_id = (*vertex_ids_at_depth).begin();
         vertex_id != (*vertex_ids_at_depth).end(); ++vertex_id) {
      if (to_be_or_not_to_be(proba_yellow)) {
        std::vector<VertexId> vertices_to_connect = get_unconnected_vertex_ids(
            *(vertex_ids_at_depth + 1), *vertex_id, graph);
        const int vertex_to_attach = get_random_vertex_id(vertices_to_connect);
        graph.add_edge(*vertex_id, vertex_to_attach, EdgeColor::Yellow);
      }
    }
  }
}

void generate_red_edges(Graph& graph) {
  if (graph.depth() < 3)
    return;

  for (auto vertex_ids_at_depth = graph.depth_map().begin();
       vertex_ids_at_depth != graph.depth_map().end() - 2;
       ++vertex_ids_at_depth) {
    for (auto vertex_id = (*vertex_ids_at_depth).begin();
         vertex_id != (*vertex_ids_at_depth).end(); ++vertex_id) {
      if (to_be_or_not_to_be(PROBA_RED)) {
        const int vertex_to_attach =
            get_random_vertex_id(*(vertex_ids_at_depth + 2));
        graph.add_edge(*vertex_id, vertex_to_attach, EdgeColor::Red);
      }
    }
  }
}

}  // namespace
namespace uni_cource_cpp {

Graph GraphGenerator::generate() const {
  auto graph = Graph();

  //Генерация нулевой вершины:
  graph.add_vertex();

  generate_gray_edges(params_.depth, params_.new_vertices_count, graph);
  generate_green_edges(graph);
  generate_yellow_edges(graph);
  generate_red_edges(graph);
  generate_blue_edges(graph);
  return graph;
}
}  // namespace uni_cource_cpp
