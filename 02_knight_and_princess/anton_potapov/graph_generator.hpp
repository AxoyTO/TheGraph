#pragma once

#include <cassert>
#include <iterator>
#include <map>
#include <random>
#include <set>

#include "graph.hpp"

constexpr double DOUBLE_COMPARISON_EPS = 1e-9;

bool is_lucky(const double probability) {
  assert(probability + DOUBLE_COMPARISON_EPS > 0 &&
         probability - DOUBLE_COMPARISON_EPS < 1 &&
         "given probability is incorrect");
  static std::knuth_b rand_engine{};
  std::mt19937 rng{rand_engine()};
  std::bernoulli_distribution bernoullu_distribution_var(probability);
  return bernoullu_distribution_var(rng);
}

class GraphGenerator {
 public:
  Graph graph;

  GraphGenerator(const int depth, const int new_vertices_num)
      : depth_(depth), new_vertices_num_(new_vertices_num) {}

  void generate_vertices() {
    graph.add_vertex();
    for (int i = 0; i <= depth_; ++i) {
      auto same_depth_vertices = graph.get_vertices_at_depth(i);
      for (const auto& current_vertex_id : same_depth_vertices) {
        for (int j = 0; j < new_vertices_num_; ++j) {
          if (depth_ > 0 && is_lucky(1.0 - (double)i / depth_)) {
            VertexId new_vertex = graph.add_vertex();
            graph.add_edge(current_vertex_id, new_vertex);
          }
        }
      }
    }
    initial_depth_distribution_ = graph.depth_distribution();
  }

  void generate_green_edges() {
    for (auto vertex_id : graph.vertex_ids()) {
      if (is_lucky(0.1)) {
        graph.add_edge(vertex_id, vertex_id, EdgeColor::Green);
      }
    }
  }

  void generate_blue_edges() {
    for (size_t cur_depth = 0; cur_depth < graph.max_depth(); ++cur_depth) {
      const auto& same_depth_vertices = graph.get_vertices_at_depth(cur_depth);
      for (const auto& vertex1_id : same_depth_vertices) {
        for (const auto& vertex2_id : same_depth_vertices) {
          if (is_lucky(0.25) && !graph.is_connected(vertex1_id, vertex2_id) &&
              graph.is_id_neighbor(vertex1_id, vertex2_id)) {
            graph.add_edge(vertex1_id, vertex2_id, EdgeColor::Blue);
          }
        }
      }
    }
  }

  void generate_yellow_edges() {
    for (size_t cur_depth = 0; cur_depth + 1 <= graph.max_depth();
         ++cur_depth) {
      const auto& cur_depth_vertices =
          initial_depth_distribution_.find(cur_depth)->second;
      const auto& next_depth_vertices =
          initial_depth_distribution_.find(cur_depth + 1)->second;
      for (const auto& cur_vertex_id : cur_depth_vertices) {
        if (depth_ > 1 && is_lucky((double)cur_depth / (depth_ - 1))) {
          std::set<VertexId> not_connected_vertices;
          for (const auto& next_vertex_id : next_depth_vertices) {
            if (!graph.is_connected(cur_vertex_id, next_vertex_id)) {
              not_connected_vertices.insert(next_vertex_id);
            }
          }
          if (!not_connected_vertices.empty()) {
            size_t rand_id = std::rand() % not_connected_vertices.size();
            auto rand_it = not_connected_vertices.begin();
            std::advance(rand_it, rand_id);
            graph.add_edge(cur_vertex_id, *rand_it, EdgeColor::Yellow);
          }
        }
      }
    }
  }

  void generate_red_edges() {
    for (size_t cur_depth = 0; cur_depth + 2 <= graph.max_depth();
         ++cur_depth) {
      const auto& cur_depth_vertices =
          initial_depth_distribution_.find(cur_depth)->second;
      const auto& next_depth_vertices =
          initial_depth_distribution_.find(cur_depth + 2)->second;
      for (const auto& cur_vertex_id : cur_depth_vertices) {
        if (is_lucky(0.33) && !next_depth_vertices.empty()) {
          size_t rand_id = std::rand() % next_depth_vertices.size();
          auto rand_it = next_depth_vertices.begin();
          std::advance(rand_it, rand_id);
          graph.add_edge(cur_vertex_id, *rand_it, EdgeColor::Red);
        }
      }
    }
  }

 private:
  const int depth_;
  const int new_vertices_num_;
  std::map<size_t, std::set<VertexId>> initial_depth_distribution_;
};
