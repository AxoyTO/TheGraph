#pragma once

#include <random>
#include "graph.hpp"
namespace uni_course_cpp {
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

  bool can_generate_vertex(float probability);
  Graph::VertexId get_random_vertex_id(
      const std::vector<Graph::VertexId>& vertex_ids);
  explicit GraphGenerator(const Params& params = Params()) : params_(params) {}

  void generate_grey_edges(Graph& graph);
  void generate_green_edges(Graph& graph);
  void generate_yellow_edges(Graph& graph);
  void generate_red_edges(Graph& graph);
  const Graph generate();

 private:
  const Params params_ = Params();
};
}  // namespace uni_course_cpp
