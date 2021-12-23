#pragma once

#include <mutex>

#include "graph.hpp"

namespace uni_cource_cpp {
struct Params {
  Params(int depth, int new_vertices_count)
      : depth(depth), new_vertices_count(new_vertices_count){};
  int depth = 0;
  int new_vertices_count = 0;
};

class GraphGenerator {
 public:
  explicit GraphGenerator(const Params& params) : params_(params){};
  Graph generate() const;

 private:
  Params params_;
  void generate_gray_edges(Graph& graph) const;
  void generate_gray_branch(Graph& graph,
                            int depth,
                            const VertexId& vertex_id,
                            std::mutex& mutex) const;
};
}  // namespace uni_cource_cpp
