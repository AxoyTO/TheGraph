#pragma once

#include <mutex>
#include "graph.hpp"

namespace uni_cpp_practice {

class GraphGenerator {
 public:
  struct Params {
    explicit Params(int depth = 0, int _new_vertices_num = 0)
        : max_depth(depth), new_vertices_num(_new_vertices_num) {}

    const int max_depth = 0;
    const int new_vertices_num = 0;
  };

  explicit GraphGenerator(const Params& params = Params()) : params_(params) {}

  Graph generate() const;

 private:
  const Params params_ = Params();
  void generate_vertices_and_gray_edges(Graph& graph,
                                        const VertexId& source_vertex_id) const;
  void generate_gray_branch(Graph& graph,
                            std::mutex& mutex,
                            const VertexId& source_vertex_id,
                            VertexDepth depth) const;
};
}  // namespace uni_cpp_practice
