#include <fstream>
#include <iostream>
#include <limits>
#include <random>
#include <sstream>
#include <string>
#include <vector>
#include "Graph.hpp"

enum { PROB_COEF = 100, GREEN_PROB = 10, BLUE_PROB = 25, RED_PROB = 33 };

std::random_device
    rd;  // Will be used to obtain a seed for the random number engine
std::default_random_engine gen(rd());
std::uniform_int_distribution<int> distrib(0, PROB_COEF);

using DepthLevelVertexId =
    std::vector<std::pair<VertexId, VertexId>>;  // vec[current_depth] =
                                                 // (first_vertex_id,
                                                 // last_vertex_id);

void generate_green_edges(Graph& g) {
  for (VertexId current_id = 0; current_id < g.get_vertices_count();
       ++current_id) {
    if ((double)distrib(gen) / PROB_COEF <
        (double)GREEN_PROB / PROB_COEF +
            std::numeric_limits<double>::epsilon()) {
      g.add_edge(current_id, current_id, "green");
    }
  }
}

void generate_blue_edges(Graph& g, const DepthLevelVertexId& depth_level) {
  Depth current_depth = 0;
  for (VertexId current_id = 0; current_id < g.get_vertices_count();
       ++current_id) {
    if (current_id != depth_level[current_depth].second) {
      if ((double)distrib(gen) / PROB_COEF <
          (double)BLUE_PROB / PROB_COEF +
              std::numeric_limits<double>::epsilon()) {
        g.add_edge(current_id, current_id + 1, "blue");
      }
    } else {
      ++current_depth;
    }
  }
}

void generate_yellow_edges(Graph& g, const DepthLevelVertexId& depth_level) {
  double yellow_edge_probability = 0.0;
  for (Depth current_depth = 0; current_depth < g.get_depth();
       ++current_depth) {
    for (VertexId parent_vertex_id = depth_level[current_depth].first;
         parent_vertex_id <= depth_level[current_depth].second;
         ++parent_vertex_id) {
      std::uniform_int_distribution<int> random_vertex(
          depth_level[current_depth + 1].first,
          depth_level[current_depth + 1].second);
      if ((double)distrib(gen) / PROB_COEF <
          yellow_edge_probability + std::numeric_limits<double>::epsilon()) {
        g.add_edge(parent_vertex_id, random_vertex(gen), "yellow");
      }
    }
    yellow_edge_probability += 1.0 / (g.get_depth() - 1);
  }
}

void generate_red_edges(Graph& g, const DepthLevelVertexId& depth_level) {
  for (Depth current_depth = 0; current_depth < g.get_depth() - 1;
       ++current_depth) {
    for (VertexId parent_vertex_id = depth_level[current_depth].first;
         parent_vertex_id <= depth_level[current_depth].second;
         ++parent_vertex_id) {
      std::uniform_int_distribution<int> random_vertex(
          depth_level[current_depth + 2].first,
          depth_level[current_depth + 2].second);
      if ((double)distrib(gen) / PROB_COEF <
          (double)RED_PROB / PROB_COEF +
              std::numeric_limits<double>::epsilon()) {
        g.add_edge(parent_vertex_id, random_vertex(gen), "red");
      }
    }
  }
}

void graph_generation(Graph& g,
                      const Depth& depth,
                      const int new_vertices_num) {
  g.set_depth(depth);

  DepthLevelVertexId depth_level;

  double new_vertext_probability = 1.0;
  g.add_vertex();
  depth_level.push_back({0, 0});

  for (Depth current_depth = 0; current_depth < depth;
       ++current_depth) {  //по всем уровням вершин
    for (VertexId parent_vertex_id = depth_level[current_depth].first;
         parent_vertex_id <= depth_level[current_depth].second;
         ++parent_vertex_id) {  //по всем порождающим вершинам
      for (int i = 0; i < new_vertices_num; ++i) {
        if ((double)distrib(gen) / PROB_COEF <
            new_vertext_probability + std::numeric_limits<double>::epsilon()) {
          g.add_vertex(current_depth + 1);  //добавляю новую вершину в граф
          g.add_edge(
              parent_vertex_id,
              g.get_vertices_count() -
                  1);  //соединяю вершину ребенка и вершину родителя(чтобы малыш
                       //не потерялся в этом страшном мире)
          if (depth_level.size() <= current_depth + 1) {
            depth_level.push_back(
                {g.get_vertices_count() - 1, g.get_vertices_count() - 1});
          } else {
            depth_level[current_depth + 1].second++;
          }
        }
      }
    }
    new_vertext_probability -= 1.0 / depth;
  }
  generate_green_edges(g);
  generate_blue_edges(g, depth_level);
  generate_yellow_edges(g, depth_level);
  generate_red_edges(g, depth_level);
}
