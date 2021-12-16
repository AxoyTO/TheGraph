#include <map>
#include <queue>
#include <set>
#include <tuple>

#include "graph.hpp"
#include "graph_traverser.hpp"

namespace uni_cource_cpp {
std::map<VertexId, int> GraphTraverser::get_updated_depths(
    const Graph& graph,
    const std::map<int, std::set<VertexId>>& known_depths,
    int start_depth) {
  std::map<VertexId, int> depths;
  std::queue<VertexId> bfs_queue;
  std::set<VertexId> used;
  if (start_depth == INIT_DEPTH) {
    const VertexId& first_vertex_id = graph.get_root_vertex_id();
    depths.emplace(first_vertex_id, 0);
    bfs_queue.push(first_vertex_id);
    used.insert(first_vertex_id);
  } else {
    int max_correct_depth = start_depth - 1;
    // copy is needed since object is always changing
    const auto vertices_at_max_correct_depth =
        known_depths.at(max_correct_depth);
    for (const auto& vertex_id : vertices_at_max_correct_depth) {
      depths.emplace(vertex_id, max_correct_depth);
      bfs_queue.push(vertex_id);
      used.insert(vertex_id);
    }
    if (max_correct_depth > 0) {
      const auto vertices_at_depth_before_max_correct =
          known_depths.at(max_correct_depth - 1);
      for (const auto& vertex_id : vertices_at_depth_before_max_correct) {
        used.insert(vertex_id);
      }
    }
  }

  while (!bfs_queue.empty()) {
    const VertexId& current_vertex_id = bfs_queue.front();
    bfs_queue.pop();
    for (const auto& connected_edge_id :
         graph.connected_edges(current_vertex_id)) {
      const Edge& connected_edge = graph.get_edge(connected_edge_id);
      const VertexId& vertex1_id = connected_edge.vertex1_id;
      const VertexId& vertex2_id = connected_edge.vertex2_id;
      const VertexId& connected_vertex_id =
          (current_vertex_id == vertex1_id ? vertex2_id : vertex2_id);
      if (used.find(connected_vertex_id) == used.end()) {
        used.insert(connected_vertex_id);
        depths[connected_vertex_id] = depths[current_vertex_id] + 1;
        bfs_queue.push(connected_vertex_id);
      }
    }
  }
  return depths;
}
}  // namespace uni_cource_cpp
