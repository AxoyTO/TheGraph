#include "graph_generator.hpp"
#include <random>
#include "graph.hpp"

namespace uni_cource_cpp {

constexpr int GREEN_EDGE_CHANCE = 10;
constexpr int BLUE_EDGE_CHANCE = 25;
constexpr int RED_EDGE_CHANCE = 33;

int random_number() {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> random_num(0, 100);

  return random_num(gen);
}

VertexId get_random_vertex_id(const std::vector<VertexId>& vertices_ids) {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> random_vertex(0, vertices_ids.size() - 1);

  return vertices_ids[random_vertex(gen)];
}

void generate_grey_edges(Graph& graph, int depth, int new_vertices_num) {
  double percent = 100.0 / (double)depth;

  VertexId curr_vert_num = 0, added_vert_num = 1;

  graph.add_vertex();
  for (int curr_depth = 0; curr_depth <= depth; curr_depth++) {
    for (int curr_depth_vert_num = 0;
         curr_depth_vert_num < graph.get_vertices_in_depth(curr_depth).size();
         curr_depth_vert_num++) {
      for (int gen_vert_num = 0; gen_vert_num < new_vertices_num;
           gen_vert_num++) {
        if ((double)random_number() > (double)curr_depth * percent) {
          graph.add_vertex();

          graph.add_edge(curr_vert_num, added_vert_num);
          added_vert_num++;
        }
      }
      curr_vert_num++;
    }
  }
}

void generate_green_edges(Graph& graph) {
  for (const auto& vertex : graph.get_vertices()) {
    if (random_number() < GREEN_EDGE_CHANCE) {
      graph.add_edge(vertex.get_id(), vertex.get_id());
    }
  }
}

void generate_blue_edges(Graph& graph) {
  for (const auto& depth : graph.get_depth_map()) {
    for (auto vertex_id = depth.begin(); vertex_id != depth.end() - 1;
         vertex_id++) {
      if (random_number() < BLUE_EDGE_CHANCE) {
        graph.add_edge(*vertex_id, *(vertex_id + 1));
      }
    }
  }
}

void generate_yellow_edges(Graph& graph) {
  double yellow_edge_percent = 100.0 / (double)(graph.get_depth() - 1);
  for (auto depth = (graph.get_depth_map()).begin();
       depth != (graph.get_depth_map()).end() - 1; depth++) {
    for (const auto& vertex_id : *depth) {
      // Вектор вершин уровнем глубже, которые не являются потомком vertex_id
      std::vector<VertexId> vert_ids_depth_deeper;
      for (const auto& vertex_id2 : *(depth + 1)) {
        if (!graph.vertices_connected(vertex_id, vertex_id2)) {
          vert_ids_depth_deeper.push_back(vertex_id2);
        }
      }
      // Проверка, что у нас есть "свободные" вершины для желтого ребра и рандом
      // выпал удачно для генерации
      if (vert_ids_depth_deeper.size() &&
          (double)random_number() <
              yellow_edge_percent *
                  (double)(depth - graph.get_depth_map().begin())) {
        graph.add_edge(vertex_id, get_random_vertex_id(vert_ids_depth_deeper));
      }
    }
  }
}

void generate_red_edges(Graph& graph) {
  for (auto depth = graph.get_depth_map().begin();
       depth != graph.get_depth_map().end() - 2; depth++) {
    for (const auto& vertex_id : *depth) {
      if (random_number() < RED_EDGE_CHANCE) {
        // Выбираем рандомом вершину 2мя уровнями глубже
        graph.add_edge(vertex_id, get_random_vertex_id(*(depth + 2)));
      }
    }
  }
}

Graph GraphGenerator::generate_graph(int depth, int new_vertices_num) const {
  Graph graph;

  generate_grey_edges(graph, depth, new_vertices_num);
  generate_green_edges(graph);
  generate_blue_edges(graph);
  generate_yellow_edges(graph);
  generate_red_edges(graph);

  return graph;
}

}  // namespace uni_cource_cpp
