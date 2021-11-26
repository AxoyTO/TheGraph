#pragma once

#include <iostream>

namespace uni_cpp_practice {

class Graph;

namespace graph_generation {

struct Params {
  Params(int _depth, int _new_vertices_num)
      : depth(_depth), new_vertices_num(_new_vertices_num){};

  int depth = 0;
  int new_vertices_num = 0;
};

Graph generate(const Params& params);

}  // namespace graph_generation

}  // namespace uni_cpp_practice
