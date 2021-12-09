#pragma once

#include <mutex>

namespace uni_cpp_practice {

class Graph;

class GraphGenerator {
 public:
  struct Params {
    Params(int _depth, int _new_vertices_num)
        : depth(_depth), new_vertices_num(_new_vertices_num){};

    int depth = 0;
    int new_vertices_num = 0;
  };

  Graph generate() const;

  GraphGenerator(const Params& params) : params_(params) {}

 private:
  Params params_;

  void generate_gray_branch(Graph& graph,
                            std::mutex& graph_mutex,
                            const VertexId& parent_vertex_id,
                            int current_depth) const;
  void generate_new_vertices(Graph& graph,
                             const VertexId& parent_vertex_id) const;
};

}  // namespace uni_cpp_practice
