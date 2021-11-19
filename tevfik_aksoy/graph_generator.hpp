#pragma once

#include "graph.hpp"

namespace uni_cpp_practice {
constexpr float GREEN_EDGE_PROBABILITY_ = 0.1;
constexpr float BLUE_EDGE_PROBABILITY_ = 0.25;
constexpr float RED_EDGE_PROBABILITY_ = 0.33;

class GraphGenerator {
 public:
  GraphGenerator(int max_depth, int new_vertices_num)
      : max_depth_(max_depth), new_vertices_num_(new_vertices_num) {
    generate();
  }

  Graph generate() const;

 private:
  int max_depth_;
  int new_vertices_num_;

  float get_random_probability() const;
  void generate_vertices_and_gray_edges(Graph& graph) const;
  void generate_green_edges(Graph& graph) const;
  void generate_blue_edges(Graph& graph) const;
  void generate_yellow_edges(Graph& graph) const;
  void generate_red_edges(Graph& graph) const;
};
}  // namespace uni_cpp_practice