#pragma once

#include "graph.hpp"

namespace uni_cource_cpp {

class GraphGenerator {
 public:
  Graph generate_graph(int depth, int new_vertices_num) const;
};

}  // namespace uni_cource_cpp
