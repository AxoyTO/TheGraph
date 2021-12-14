#pragma once
#include <assert.h>
#include <stdlib.h>
#include <time.h>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include "graph.hpp"

class Graph_generator {
 public:
  struct Params {
    explicit Params(Depth _depth = 0, int _new_vertices_num = 0)
        : depth(_depth), new_vertices_num(_new_vertices_num) {}

    const Depth depth = 0;
    const int new_vertices_num = 0;
  };

  explicit Graph_generator(const Params& params = Params()) : params_(params){};
  void generate_vertices(Graph& graph) const;
  Graph generate_graph() const;

 private:
  const Params params_ = Params();
};
