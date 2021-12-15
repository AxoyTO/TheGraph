#pragma once

#include <map>
#include <queue>
#include <tuple>
#include "graph.hpp"

namespace uni_cource_cpp {
class GraphTraverser {
 public:
  static std::map<VertexId, int> get_updated_depths(
      const Graph& graph,
      const std::map<int, std::set<VertexId>>& known_depths,
      int start_depth);
  static std::set<VertexId> get_updated_vertices_at_depth(
      const Graph& graph,
      const std::map<int, std::set<VertexId>>& known_depths,
      int depth,
      int start_depth);
  static int get_updated_vertex_depth(
      const Graph& graph,
      const std::map<int, std::set<VertexId>>& known_depths,
      VertexId vertex_id,
      int start_depth);
  static int get_max_depth(
      const Graph& graph,
      const std::map<int, std::set<VertexId>>& known_depths,
      int start_depth);

 private:
  static std::
      tuple<std::map<VertexId, int>, std::queue<VertexId>, std::set<VertexId>>
      init_bfs(const Graph& graph,
               const std::map<int, std::set<VertexId>>& known_depths,
               int start_depth);
};
}  // namespace uni_cource_cpp
