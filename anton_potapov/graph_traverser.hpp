#pragma once

#include <map>
#include <queue>
#include <tuple>
#include <vector>
#include "graph.hpp"

namespace uni_cource_cpp {
class GraphTraverser {
 public:
  static std::map<VertexId, int> get_updated_depths(
      const Graph& graph,
      const std::map<int, std::set<VertexId>>& known_depths,
      int start_depth);
};
}  // namespace uni_cource_cpp
