#include "graph_traverser.hpp"

namespace {
const int MAX_WORKERS_COUNT = std::thread::hardware_concurrency();
constexpr int MAX_DISTANCE = INT_MAX;
}  // namespace

namespace uni_course_cpp {
  
GraphPath GraphTraverser::find_shortest_path(const VertexId& source_vertex_id,
                              const VertexId& destination_vertex_id) const {
  assert(graph_.has_vertex(source_vertex_id));
  assert(graph_.has_vertex(destination_vertex_id));

  const int vertices_count = graph.get_vertices().size()
  std::vector<Distance> distances(vertices_count, MAX_DISTANCE);
  distances[source_vertex_id] = 0;

  VertexId linked_vertex_id;

  std::vector<bool> marked(vertices_count, false);
  std::vector<VertexId> vertices_to_check;

  VertexId closest_vertex_id = source_vertex_id;

  // Distance min_distance_to_unmarked = 0;
  while (std::find(marked.begin(), marked.end(), false) != marked.end()) {
    for (const auto& edge_id : graph.get_vertex(closest_vertex_id).get_edge_ids()) {
      const auto& edge = graph.get_edge(edge_id);
      if (edge.color != Edge::Color::Green) {
        if (edge.vertex1_id != closest_vertex_id) {
          linked_vertex_id = edge.vertex1_id;
        }
        else linked_vertex_id = edge.vertex2_id;
        if (!marked[linked_vertex_id]) {
          vertices_to_check.push_back(linked_vertex_id);
        }
      }
    }

    for (const auto& vertex_id : vertices_to_check) {
      if (distances[closest_vertex_id] + 1 < distances[vertex_id]) {
        distances[vertex_id] = distances[closest_vertex_id] + 1;
      }
    }
    vertices_to_check.clear();

    marked[closest_vertex_id] = true;
    closest_vertex_id = std::min_element(distances.begin(), distances.end()) - distances.begin();
  }
}

std::vector<GraphPath> GraphTraverser::find_all_paths() const {

}
}  // namespace uni_course_cpp
