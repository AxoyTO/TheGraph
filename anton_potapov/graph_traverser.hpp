#pragma once

#include <map>
#include "graph.hpp"

namespace uni_cource_cpp {
class GraphTraverser {
 public:
  static std::map<VertexId, int> get_updated_depths(const Graph& graph,
                                                    int start_depth);
  static std::map<VertexId, int> get_depths(const Graph& graph);
  static int get_updated_vertex_depth(const Graph& Graph,
                                      VertexId vertex_id,
                                      int start_depth);
  static int get_vertex_depth(const Graph& Graph, VertexId vertex_id);
};
}  // namespace uni_cource_cpp
