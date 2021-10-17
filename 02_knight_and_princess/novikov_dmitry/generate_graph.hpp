#ifndef GENERATE_GRAPH_HPP
#define GENERATE_GRAPH_HPP

#include <fstream>
#include <iostream>
#include <limits>
#include <random>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>
#include "graph.hpp"

namespace generation {

bool is_lucky(const double& probability) {
  const int probability_coef = 100;
  std::random_device
      rd;  // Will be used to obtain a seed for the random number engine
  std::default_random_engine gen(rd());
  std::uniform_int_distribution<int> distrib(0, probability_coef);
  if ((double)distrib(gen) / probability_coef <
      probability + std::numeric_limits<double>::epsilon()) {
    return true;
  }
  return false;
}

int get_random_idx(const int& size) {
  std::random_device
      rd;  // Will be used to obtain a seed for the random number engine
  std::default_random_engine gen(rd());
  std::uniform_int_distribution<int> distrib(0, size - 1);
  return distrib(gen);
}

void generate_green_edges(Graph& g, const double& probability) {
  for (VertexId current_id = 0; current_id < g.get_vertices_count();
       ++current_id) {
    if (is_lucky(probability)) {
      g.add_edge(current_id, current_id, Color::GREEN);
    }
  }
}

void generate_blue_edges(Graph& graph, const double& probability) {
  for (Depth current_depth = 1; current_depth <= graph.get_depth();
       ++current_depth) {
    const auto& vertices_at_depth = graph.get_vertices_at_depth(current_depth);
    for (int idx = 0; idx < vertices_at_depth.size(); ++idx) {
      if ((idx + 1 < vertices_at_depth.size()) && is_lucky(probability) &&
          !graph.check_binding(vertices_at_depth[idx],
                               vertices_at_depth[idx + 1])) {
        graph.add_edge(vertices_at_depth[idx], vertices_at_depth[idx + 1],
                       Color::BLUE);
      }
    }
  }
}

void generate_yellow_edges(Graph& graph, const Depth& depth) {
  double yellow_edge_probability = 1.0 / (depth - 1);
  for (Depth current_depth = 1; current_depth < graph.get_depth();
       ++current_depth) {
    const auto& vertices_at_depth = graph.get_vertices_at_depth(current_depth);
    const auto& vertices_at_next_depth =
        graph.get_vertices_at_depth(current_depth + 1);
    for (const auto& current_vertex_id : vertices_at_depth) {
      if (is_lucky(yellow_edge_probability)) {
        std::vector<VertexId> not_binded_vertices;
        for (const auto& next_vertex_id : vertices_at_next_depth) {
          if (!graph.check_binding(current_vertex_id, next_vertex_id)) {
            not_binded_vertices.push_back(next_vertex_id);
          }
        }
        if (not_binded_vertices.size()) {
          int idx = get_random_idx(not_binded_vertices.size());
          graph.add_edge(current_vertex_id, not_binded_vertices[idx],
                         Color::YELLOW);
        }
      }
    }
    yellow_edge_probability += 1.0 / (depth - 1);
  }
}

void generate_red_edges(Graph& graph, const double& probability) {
  for (Depth current_depth = 0; current_depth < graph.get_depth() - 1;
       ++current_depth) {
    const auto& vertices_at_depth = graph.get_vertices_at_depth(current_depth);
    const auto& vertices_at_next_depth =
        graph.get_vertices_at_depth(current_depth + 2);
    for (const auto& current_vertex_id : vertices_at_depth) {
      if (is_lucky(probability)) {
        int idx = get_random_idx(vertices_at_next_depth.size());
        graph.add_edge(current_vertex_id, vertices_at_next_depth[idx],
                       Color::RED);
      }
    }
  }
}

void generate_gray_edges(Graph& graph,
                         const Depth& depth,
                         const int new_vertices_num) {
  double new_vertext_probability = 1.0;
  graph.add_vertex();

  for (Depth current_depth = 0;
       current_depth < depth && current_depth <= graph.get_depth();
       ++current_depth) {  //по всем уровням вершин
    for (VertexId parent_vertex_id : graph.get_vertices_at_depth(
             current_depth)) {  //по всем порождающим вершинам
      for (int i = 0; i < new_vertices_num; ++i) {
        if (is_lucky(new_vertext_probability)) {
          const VertexId& new_vertex_id = graph.add_vertex(
              current_depth + 1);  //добавляю новую вершину в граф
          graph.add_edge(parent_vertex_id,
                         new_vertex_id);  //соединяю вершину ребенка и вершину
                                          //родителя(чтобы малыш не потерялся в
                                          //этом страшном мире)
        }
      }
    }
    new_vertext_probability -= 1.0 / depth;
  }
}

void generate_graph(Graph& graph,
                    const Depth& depth,
                    const int new_vertices_num) {
  static const std::unordered_map<Color, double> color_probability_map = {
      {Color::GREEN, 0.1}, {Color::BLUE, 0.25}, {Color::RED, 0.33}};
  generate_gray_edges(graph, depth, new_vertices_num);
  generate_green_edges(graph, color_probability_map.at(Color::GREEN));
  generate_blue_edges(graph, color_probability_map.at(Color::BLUE));
  generate_yellow_edges(graph, depth);
  generate_red_edges(graph, color_probability_map.at(Color::RED));
}
}  // namespace generation

#endif  // GENERATE_GRAPH_HPP
