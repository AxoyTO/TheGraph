#pragma once

#include <cassert>
#include <iterator>
#include <limits>
#include <map>
#include <random>
#include <set>
#include <utility>

#include "graph.hpp"

constexpr float DOUBLE_COMPARISON_EPS = std::numeric_limits<float>::epsilon();
constexpr float GREEN_EDGE_PROB = 0.1;
constexpr float BLUE_EDGE_PROB = 0.25;
constexpr float RED_EDGE_PROB = 0.33;

bool is_lucky(float probability) {
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
  GraphGenerator(const int depth, const int new_vertices_num)
      : depth_(depth), new_vertices_num_(new_vertices_num) {}

  Graph& generate_graph() {
    this->generate_vertices();
    this->generate_green_edges();
    this->generate_blue_edges();
    this->generate_yellow_edges();
    this->generate_red_edges();
    return this->graph_;
  }

 private:
  void generate_vertices() {
    graph_.add_vertex();
    for (int i = 0; i <= graph_.max_depth(); ++i) {
      auto same_depth_vertices = graph_.get_vertices_at_depth(i);
      for (const auto& current_vertex_id : same_depth_vertices) {
        for (int j = 0; j < new_vertices_num_; ++j) {
          if (depth_ > 0 && is_lucky(1.0 - (double)i / depth_)) {
            VertexId new_vertex = graph_.add_vertex();
            graph_.add_edge(current_vertex_id, new_vertex);
          }
        }
      }
    }
    initial_depth_distribution_ = graph_.depth_distribution();
  }

  void generate_green_edges() {
    for (const auto& [vertex_id, vertex] : graph_.vertices()) {
      if (is_lucky(GREEN_EDGE_PROB)) {
        graph_.add_edge(vertex_id, vertex_id, EdgeColor::Green);
      }
    }
  }

  void generate_blue_edges() {
   for (int cur_depth = 0; cur_depth <= graph_.max_depth(); ++cur_depth) {
      const auto& same_depth_vertices = graph_.get_vertices_at_depth(cur_depth);
      for (auto it = same_depth_vertices.begin(); std::next(it) != same_depth_vertices.end(); ++it) {
        const auto& vertex1_id = *it;
        const auto& vertex2_id = *std::next(it);
        if (is_lucky(BLUE_EDGE_PROB) &&
            !graph_.is_connected(vertex1_id, vertex2_id)) {
          graph_.add_edge(vertex1_id, vertex2_id, EdgeColor::Blue);
        }
      }
    }
  }

  void generate_yellow_edges() {
    for (int cur_depth = 0; cur_depth + 1 <= graph_.max_depth(); ++cur_depth) {
      const auto& cur_depth_vertices =
          initial_depth_distribution_.find(cur_depth)->second;
      const auto& next_depth_vertices =
          initial_depth_distribution_.find(cur_depth + 1)->second;
      for (const auto& cur_vertex_id : cur_depth_vertices) {
        if (depth_ > 1 && is_lucky((double)cur_depth / (depth_ - 1))) {
          std::set<VertexId> not_connected_vertices;
          for (const auto& next_vertex_id : next_depth_vertices) {
            if (!graph_.is_connected(cur_vertex_id, next_vertex_id)) {
              not_connected_vertices.insert(next_vertex_id);
            }
          }
          if (!not_connected_vertices.empty()) {
            size_t rand_id = std::rand() % not_connected_vertices.size();
            auto rand_it = not_connected_vertices.begin();
            std::advance(rand_it, rand_id);
            graph_.add_edge(cur_vertex_id, *rand_it, EdgeColor::Yellow);
          }
        }
      }
    }
  }

  void generate_red_edges() {
    for (int cur_depth = 0; cur_depth + 2 <= graph_.max_depth(); ++cur_depth) {
      const auto& cur_depth_vertices =
          initial_depth_distribution_.find(cur_depth)->second;
      const auto& next_depth_vertices =
          initial_depth_distribution_.find(cur_depth + 2)->second;
      for (const auto& cur_vertex_id : cur_depth_vertices) {
        if (is_lucky(RED_EDGE_PROB) && !next_depth_vertices.empty()) {
          size_t rand_id = std::rand() % next_depth_vertices.size();
          auto rand_it = next_depth_vertices.begin();
          std::advance(rand_it, rand_id);
          graph_.add_edge(cur_vertex_id, *rand_it, EdgeColor::Red);
        }
      }
    }
  }

  Graph graph_;
  const int depth_;
  const int new_vertices_num_;
  std::map<int, std::set<VertexId>> initial_depth_distribution_;
};
