#pragma once

#include <mutex>
#include "graph.hpp"

namespace uni_cource_cpp {

class GraphGenerator {
 public:
  struct Params {
   public:
    explicit Params(Graph::Depth depth = 0, int new_vertices_count = 0)
        : depth_(depth), new_vertices_count_(new_vertices_count) {}

    Graph::Depth depth() const { return depth_; }
    int new_vertices_count() const { return new_vertices_count_; }

   private:
    Graph::Depth depth_ = 0;
    int new_vertices_count_ = 0;
  };

  explicit GraphGenerator(const Params& params = Params()) : params_(params) {}

  Graph generate() const;

  void generate_gray_branch(Graph& graph,
                            const Graph::VertexId& parent_vertex_id,
                            std::mutex& new_edge_mutex,
                            const Graph::Depth& depth,
                            int new_vertices_num) const;

  void generate_gray_edges(Graph& graph,
                           const Graph::VertexId& root_vertex_id,
                           const Graph::Depth& depth,
                           int new_vertices_count) const;

 private:
  const Params params_ = Params();
};

}  // namespace uni_cource_cpp
