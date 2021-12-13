#pragma once

#include <map>
#include "graph.hpp"

class GraphTraverser {
 public:
  static std::map<VertexId, int> dynamic_bfs(
      const Graph& graph,
      int start_depth);
  static std::map<VertexId, int> bfs_get_depth_distribution(const Graph& graph);
  int get_vertex_depth(const Graph& Graph, VertexId vertex_id, int start_depth);
  int get_vertex_depth(const Graph& Graph, VertexId vertex_id);
};
