#pragma once

#include <mutex>
#include "graph.hpp"

namespace uni_cource_cpp {

class GraphGenerator {
 public:
  GraphGenerator(int depth, int new_vertices_num)
      : depth_(depth), new_vertices_num_(new_vertices_num) {}
  Graph generate_graph() const;

 private:
  void generate_vertices(Graph& graph, const VertexId& first_vertex_id) const;
  void generate_grey_edge(Graph& graph,
                          const VertexId parent_vertex_id,
                          int parent_depth,
                          std::mutex& lock_graph) const;
  int depth_;
  int new_vertices_num_;
};

}  // namespace uni_cource_cpp
