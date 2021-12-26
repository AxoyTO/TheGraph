#include "graph_traverser.hpp"
#include <climits>
#include "graph.hpp"
#include "graph_path.hpp"

namespace uni_course_cpp {

GraphPath find_shortest_path(Graph::VertexId source_vertex_id,
                             Graph::VertexId destination_vertex_id) const {
  std::unordered_map<Graph::VertexId, GraphDistance::Distance> distance_map;
  for (const auto& vertex : graph_.vertices()) {
    if (vertex.id == source_vertex_id)
      distance_map[vertex.id] = 0;
    else
      distance_map[vertex.id] = INT_MAX;
  }
  std::queue<Graph::Vertex> vertices_queue;
  vertices_queue.push(source_vertex);

}
}  // namespace uni_course_cpp
