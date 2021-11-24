#pragma once

namespace uni_cpp_practice {

class Graph;

namespace graph_generation {

struct Params {
  Params() = default;
  Params(int _depth, int _new_vertices_num)
      : depth(_depth), new_vertices_num(_new_vertices_num){};

  Params(const Params& _params)
      : depth(_params.depth), new_vertices_num(_params.new_vertices_num){};

  int depth = 0;
  int new_vertices_num = 0;
};

Graph generate(Params params);

}  // namespace graph_generation

}  // namespace uni_cpp_practice
