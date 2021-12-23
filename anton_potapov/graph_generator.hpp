#pragma once

#include "graph.hpp"

namespace uni_cource_cpp {
class GraphGenerator {
 public:
  struct Params {
    explicit Params(int depth_val = 0, int new_vertices_num_val = 0);
    const int depth;
    const int new_vertices_num;
  };
  explicit GraphGenerator(const Params& params = Params());
  Graph generate_graph() const;

 private:
  const Params params_{};
  void generate_vertices_branch(Graph& graph,
                                std::mutex& graph_mutex,
                                const VertexId& previous_vertex_id,
                                int current_depth) const;
  void generate_vertices(Graph& graph) const;
  void generate_green_edges(Graph& graph,
                            std::mutex& graph_read_write_mutex) const;
  void generate_blue_edges(Graph& graph,
                           std::mutex& graph_read_write_mutex) const;
  void generate_yellow_edges(Graph& graph,
                             std::mutex& graph_read_write_mutex) const;
  void generate_red_edges(Graph& graph) const;
};
}  // namespace uni_cource_cpp
