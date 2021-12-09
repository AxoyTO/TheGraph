#pragma once

#include "graph.hpp"

namespace uni_course_cpp {

class GraphGenerator {
 public:
  struct Params {
    explicit Params(int _depth = 0, int _new_vertices_num = 0)
        : depth(_depth), new_vertices_num(_new_vertices_num) {}

    const int depth = 0;
    const int new_vertices_num = 0;
  };

  explicit GraphGenerator(const Params& params = Params()) : params_(params) {}

  Graph generate() const;

 private:
  void add_gray_edges(Graph& graph) const;
  void add_green_edges(Graph& graph) const;
  void add_yellow_edges(Graph& graph) const;
  void add_red_edges(Graph& graph) const;
  const Params params_ = Params();
};
}  // namespace uni_course_cpp
