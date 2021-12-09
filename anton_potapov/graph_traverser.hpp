#pragma once

#include <map>
#include "graph.hpp"

class GraphTraverser {
 public:
  static std::map<VertexId, int> dynamic_bfs(const Graph& graph,
                                             int start_depth);
};
