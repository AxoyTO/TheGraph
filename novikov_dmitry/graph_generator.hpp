#pragma once

#include "graph.hpp"

namespace uni_cpp_practice {

class GraphGenerator {
 public:
  GraphGenerator(int max_depth, int new_vertices_num)
      : max_depth_(max_depth), new_vertices_num_(new_vertices_num) {}

  Graph generate() const;

 private:
  int max_depth_;
  int new_vertices_num_;

  void generate_gray_edges(Graph& graph) const;
  void generate_green_edges(Graph& graph) const;
  void generate_blue_edges(Graph& graph) const;
  void generate_yellow_edges(Graph& graph) const;
  void generate_red_edges(Graph& graph) const;
};
}  // namespace uni_cpp_practice
