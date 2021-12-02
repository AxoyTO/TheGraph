#pragma once

#include <mutex>
#include "graph.hpp"

namespace uni_cpp_practice {

class GraphGenerator {
 public:
  struct Params {
    explicit Params(Depth _depth = 0, int _new_vertices_num = 0)
        : depth(_depth), new_vertices_num(_new_vertices_num) {}

    const Depth depth = 0;
    const int new_vertices_num = 0;
  };

  explicit GraphGenerator(const Params& params = Params()) : params_(params) {}

  Graph generate() const;

 private:
  const Params params_ = Params();
  void generate_gray_edges(Graph& graph,
                           const VertexId& parent_vertex_id) const;
  void generate_gray_branch(Graph& graph,
                            std::mutex& mutex_add,
                            const VertexId& parent_vertex_id,
                            const Depth current_depth) const;
};
}  // namespace uni_cpp_practice
