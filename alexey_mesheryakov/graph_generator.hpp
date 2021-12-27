#pragma once
#include <mutex>
#include "graph.hpp"
namespace uni_course_cpp {
class GraphGenerator {
 public:
  struct Params {
    explicit Params(const Depth& _depth = 0, int _new_vertices_num = 0)
        : depth(_depth), new_vertices_num(_new_vertices_num) {}

    const Depth depth = 0;
    const int new_vertices_num = 0;
  };

  explicit GraphGenerator(const Params& params = Params()) : params_(params){};
  Graph generate_graph() const;

 private:
  const Params params_ = Params();
  void generate_gray_edges(Graph& graph) const;
  void generate_gray_branch(Graph& graph,
                            const VertexId& from_vertex_id,
                            const Depth& depth,
                            std::mutex& graph_mutex) const;
};
}  // namespace uni_course_cpp
