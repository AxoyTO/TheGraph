#pragma once

#include "graph.hpp"

class GraphGenerator {
 public:
  GraphGenerator(int max_depth, int new_vertices_num)
      : max_depth_(max_depth), new_vertices_num_(new_vertices_num) {
    generate();
  }

  float get_random_probability();

  uni_cpp_practice::VertexId get_random_vertex_id(
      const std::vector<uni_cpp_practice::VertexId>& vertices);

  void generate_vertices_and_gray_edges(Graph& graph);

  void generate_green_edges(Graph& graph);

  void generate_blue_edges(Graph& graph);

  void generate_yellow_edges(Graph& graph);

  void generate_red_edges(Graph& graph);

  const Graph generate();

  std::vector<uni_cpp_practice::VertexId> filter_connected_vertices(
      const uni_cpp_practice::VertexId& id,
      const std::vector<uni_cpp_practice::VertexId>& vertex_ids,
      const Graph& graph);

 private:
  int max_depth_;
  int new_vertices_num_;
  const float GREEN_EDGE_PROBABILITY_ = 0.1;
  const float BLUE_EDGE_PROBABILITY_ = 0.25;
  const float RED_EDGE_PROBABILITY_ = 0.33;
};
