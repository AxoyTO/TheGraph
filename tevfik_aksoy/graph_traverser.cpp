#include "graph_traverser.hpp"
#include <climits>
#include <iostream>
#include <list>
#include <queue>

using Pair = std::pair<int, int>;

namespace uni_cpp_practice {
GraphTraverser::GraphTraverser(const Graph& graph)
    : vertices_(graph.get_vertices()), edges_(graph.get_edges()) {
  for (const auto& destination_vertex_id :
       graph.get_vertices_in_depth(graph.depth())) {
    find_shortest_path(0, destination_vertex_id);
  }
}

void GraphTraverser::parse_shortest_path(std::vector<VertexId> path_vertices,
                                         VertexId vertex_id) {
  if (path_vertices[vertex_id] == -1) {
    path_.vertex_ids.push_back(vertex_id);
    return;
  }

  parse_shortest_path(path_vertices, path_vertices[vertex_id]);
  path_.vertex_ids.push_back(vertex_id);
}

void GraphTraverser::save_shortest_paths(std::vector<int> distances,
                                         VertexId destination,
                                         std::vector<int> path_vertices) {
  parse_shortest_path(path_vertices, destination);
  path_.distance = distances[destination];
  shortest_paths_.emplace_back(path_);
  path_.vertex_ids.clear();
}

std::optional<GraphTraverser::Path> GraphTraverser::find_shortest_path(
    VertexId source_vertex_id,
    VertexId destination_vertex_id) {
  if (vertices_.size() == 1)
    return std::nullopt;

  std::priority_queue<Pair, std::vector<Pair>, std::greater<Pair>>
      priority_queue;

  std::vector<Distance> distances(vertices_.size(), INT_MAX);
  std::vector<VertexId> dummy_path_vertices;

  for (int i = 0; i < vertices_.size(); i++) {
    dummy_path_vertices.push_back(-1);
  }

  priority_queue.push(std::make_pair(source_vertex_id, 0));
  distances[source_vertex_id] = 0;

  while (!priority_queue.empty()) {
    VertexId closest_vertex = priority_queue.top().first;
    priority_queue.pop();
    for (const auto& vertices_and_distances :
         get_adjacent_vertices_and_distances(closest_vertex)) {
      VertexId vertex_id = vertices_and_distances.first;
      Distance distance = vertices_and_distances.second;
      if (distances[vertex_id] > distances[closest_vertex] + distance) {
        dummy_path_vertices[vertex_id] = closest_vertex;
        distances[vertex_id] = distances[closest_vertex] + distance;
        priority_queue.push(std::make_pair(vertex_id, distances[vertex_id]));
      }
    }
  }

  save_shortest_paths(distances, destination_vertex_id, dummy_path_vertices);
  return path_;
}

std::vector<GraphTraverser::Path> GraphTraverser::get_shortest_paths() const {
  return shortest_paths_;
}

std::vector<std::pair<VertexId, EdgeId>>
GraphTraverser::get_adjacent_vertices_and_distances(const VertexId& vertex_id) {
  std::vector<std::pair<VertexId, EdgeId>> adjacent_vertices;
  auto vertex_edges = vertices_[vertex_id].get_edge_ids();
  for (const auto& edge_id : vertex_edges) {
    if (edges_[edge_id].color == Edge::Color::Green)
      continue;
    else {
      if (edges_[edge_id].source == vertex_id)
        adjacent_vertices.push_back(
            std::make_pair(edges_[edge_id].destination, 1));
      else
        adjacent_vertices.push_back(std::make_pair(edges_[edge_id].source, 1));
    }
  }

  return adjacent_vertices;
}
}  // namespace uni_cpp_practice
