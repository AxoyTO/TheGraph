#pragma once

#include "graph.hpp"

#include <mutex>

namespace uni_cource_cpp {
class GraphGenerator {
 public:
  struct Params {
    explicit Params(int _depth = 0, int _new_vertices_num = 0)
        : depth(_depth), new_vertices_num(_new_vertices_num) {}

    const int depth = 0;
    const int new_vertices_num = 0;
  };

  explicit GraphGenerator(const Params& params = Params()) : params_(params) {}

  Graph generate() const;

 private:
  void generate_grey_edges(Graph& graph) const;
  void generate_gray_branch(Graph& graph,
                            Depth depth,
                            const VertexId& parent_vertex_id,
                            std::mutex& mutex) const;

  const Params params_ = Params();
};
}  // namespace uni_cource_cpp
