#pragma once

#include <mutex>
#include "graph.hpp"

namespace uni_course_cpp {

class GraphGenerator {
 public:
  struct Params {
    explicit Params(int _depth = 0, int _new_vertices_count = 0)
        : depth(_depth), new_vertices_count(_new_vertices_count) {}

    const int depth = 0;
    const int new_vertices_count = 0;
  };

  explicit GraphGenerator(const Params& params = Params()) : params_(params) {}

  Graph generate() const;

 private:
  void generate_gray_edges(Graph& graph) const;
  void generate_green_edges(Graph& graph, std::mutex& mutex) const;
  void generate_yellow_edges(Graph& graph, std::mutex& mutex) const;
  void generate_red_edges(Graph& graph, std::mutex& mutex) const;
  void generate_gray_branch(Graph& graph,
                            int depth,
                            const VertexId& parent_vertex_id,
                            std::mutex& mutex) const;
  const Params params_ = Params();
};
}  // namespace uni_course_cpp
