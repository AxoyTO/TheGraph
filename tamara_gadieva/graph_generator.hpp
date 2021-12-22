#pragma once
#include <mutex>
#include "graph.hpp"

namespace uni_cource_cpp {
class GraphGenerator {
 public:
  struct Params {
    const int depth = 0;
    const int new_vertices_num = 0;

    explicit Params(int _depth = 0, int _new_vertices_num = 0)
        : depth(_depth), new_vertices_num(_new_vertices_num) {}
  };

  explicit GraphGenerator(const Params& params = Params()) : params_(params) {}

  Graph generate() const;

 private:
  const Params params_ = Params();
  void generate_gray_edges(Graph& graph, const VertexId& from_vertex_id) const;
  void generate_gray_branch(Graph& graph, const VertexId& from_vertex_id, 
    int current_depth, std::mutex& graph_mutex) const;
};
}  // namespace uni_cource_cpp
