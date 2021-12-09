#pragma once

#include "graph.hpp"

namespace uni_cpp_practice {
class GraphGenerator {
 public:
  struct Params {
    explicit Params(int init_depth = 0, int init_new_vertices_num = 0)
        : depth(init_depth), new_vertices_num(init_new_vertices_num) {}

    const int depth = 0;
    const int new_vertices_num = 0;
  };

  explicit GraphGenerator(const Params& params = Params()) : params_(params) {}

  Graph generate_random_graph() const;

 private:
  const Params params_ = Params();

  void generate_grey_edges(Graph& graph, int depth, int new_vertices_num) const;

  void generate_green_edges(Graph& graph) const;
  void generate_blue_edges(Graph& graph) const;
  void generate_yellow_edges(Graph& graph) const;
  void generate_red_edges(Graph& graph) const;
};
}  // namespace uni_cpp_practice
