#pragma once

#include <mutex>
#include "graph.hpp"

namespace uni_cource_cpp {

class GraphGenerator {
 public:
  struct Params {
    int depth = 0;
    int new_vertices_num = 0;
    Params() {}
    Params(int input_depth, int input_new_vertices_num)
        : depth(input_depth), new_vertices_num(input_new_vertices_num) {}
  } params;

  GraphGenerator() {}
  GraphGenerator(const Params& input_params) : params(input_params) {}
  Graph generate_graph() const;

 private:
  void generate_vertices(Graph& graph, const VertexId& first_vertex_id) const;
  void generate_grey_edge(Graph& graph,
                          const VertexId parent_vertex_id,
                          int parent_depth,
                          std::mutex& lock_graph) const;
};

}  // namespace uni_cource_cpp
