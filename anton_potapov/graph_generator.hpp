#pragma once

#include "graph.hpp"

void generate_vertices(Graph& graph, int depth, int new_vertices_num);
void generate_green_edges(Graph& graph);
void generate_blue_edges(Graph& graph);
void generate_yellow_edges(Graph& graph);
void generate_red_edges(Graph& graph);

class GraphGenerator {
 public:
  struct Params {
    explicit Params(int depth_val = 0, int new_vertices_num_val = 0);
    const int depth;
    const int new_vertices_num;
  };
  explicit GraphGenerator(const Params& params = Params());
  Graph generate_graph() const;

 private:
  const Params params_{};
};
