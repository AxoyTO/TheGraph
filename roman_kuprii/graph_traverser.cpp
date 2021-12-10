#include <iostream>

#include <cassert>
#include <optional>
#include <queue>
#include <vector>

#include "graph.hpp"
#include "graph_traverser.hpp"

namespace uni_cpp_practice {

namespace {

constexpr int UNVISITED = 0;
constexpr int VISITED = 1;
// constexpr int MAX_DISTANCE = INT_MAX;
constexpr int MAX_DISTANCE = 100000;

std::optional<Vertex> get_vertex_from_id(Graph& graph,
                                         const VertexId& vertex_id) {
  const auto& vertices = graph.get_vertices();
  for (const auto& vertex : vertices)
    if (vertex.get_id() == vertex_id)
      return vertex;
  return std::nullopt;
}

std::optional<Edge> get_edge_from_id(Graph& graph, const EdgeId& edge_id) {
  const auto& edges = graph.get_edges();
  for (const auto& edge : edges)
    if (edge.id == edge_id)
      return edge;
  return std::nullopt;
}

}  // namespace

std::optional<GraphTraverser::Path> GraphTraverser::find_shortest_path(
    Graph& graph,
    const VertexId& source_vertex_id,
    const VertexId& destination_vertex_id) const {
  // unvisited vertices
  std::vector<VertexId> vertices(graph.get_vertices_num(), UNVISITED);
  vertices[source_vertex_id] = VISITED;
  // get source vertex
  const auto& source_vertex = get_vertex_from_id(graph, source_vertex_id);
  assert(source_vertex.has_value());
  // create queue
  std::queue<Vertex> vertices_queue;
  vertices_queue.push(source_vertex.value());
  // creante distances
  std::vector<Distance> distance(graph.get_vertices_num(), MAX_DISTANCE);
  distance[source_vertex_id] = 0;

  while (!vertices_queue.empty()) {
    const auto current_vertex = vertices_queue.front();
    vertices_queue.pop();

    //    if (current_vertex.get_id() == destination_vertex_id)
    //      return std::nullopt;

    // check all outcoming edges
    for (const auto& edge_id : current_vertex.get_edges_ids()) {
      const auto& edge = get_edge_from_id(graph, edge_id);
      assert(edge.has_value());
      const auto& next_vertex_id = edge->connected_vertices.back();
      const auto& next_vertex = get_vertex_from_id(graph, next_vertex_id);
      assert(next_vertex.has_value());
      // update distances
      if (distance[current_vertex.get_id()] + 1 < distance[next_vertex_id]) {
        if (distance[next_vertex_id] != MAX_DISTANCE) {
          //            for (auto it = vertices_queue.begin(); it !=
          //            vertices_queue.end(); it++) {
          //                vertices_queue.erase(next_vertex);
          vertices_queue.pop();
          //                break;
          //            }
        }
        vertices_queue.push(next_vertex.value());
        distance[next_vertex_id] = distance[current_vertex.get_id()] + 1;
      }
    }
  }

  for (const auto& i : distance)
    std::cout << i;
  std::cout << std::endl;

  return std::nullopt;
}

}  // namespace uni_cpp_practice
