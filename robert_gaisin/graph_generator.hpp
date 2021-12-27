#pragma once

#include <mutex>

#include "graph.hpp"

namespace uni_course_cpp {

class GraphGenerator {
 public:
  struct Params {
    Params(int _depth = 0, int _new_vertices_count = 0)
        : depth(_depth), new_vertices_count(_new_vertices_count) {}

    const int depth = 0;
    const int new_vertices_count = 0;
  };
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
}  // namespace uni_course_cpp
