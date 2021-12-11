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
    std::cout << "Source: 0 --- Destination: " << destination_vertex_id << "\n";
    find_shortest_path(0, destination_vertex_id);
  }
}

void printPath(std::vector<int> path_vertices, int j) {
  if (path_vertices[j] == -1)
    return;

  printPath(path_vertices, path_vertices[j]);

  printf("%d ", j);
}

void printSolution(std::vector<int> dist,
                   int V,
                   std::vector<int> path_vertices) {
  int src = 0;
  printf("Vertex\t Distance\tPath");
  for (int i = 1; i < V; i++) {
    printf("\n%d -> %d \t\t %d\t\t%d ", src, i, dist[i], src);
    printPath(path_vertices, i);
  }
  std::cout << "\n";
}

std::optional<GraphTraverser::Path> GraphTraverser::find_shortest_path(
    VertexId source_vertex_id,
    VertexId destination_vertex_id) {
  std::priority_queue<Pair, std::vector<Pair>, std::greater<Pair>> pq;

  std::vector<int> dist(vertices_.size(), INT_MAX);

  for (int i = 0; i < vertices_.size(); i++) {
    path_.vertex_ids.push_back(-1);
  }

  pq.push(std::make_pair(0, source_vertex_id));
  dist[source_vertex_id] = 0;

  while (!pq.empty()) {
    int u = pq.top().second;
    pq.pop();
    for (const auto& i : get_adjacent_with_distances(u)) {
      int vertex = i.first;
      int distance = i.second;
      if (dist[vertex] > dist[u] + distance) {
        path_.vertex_ids[vertex] = u;
        dist[vertex] = dist[u] + distance;
        pq.push(std::make_pair(dist[vertex], vertex));
      }
    }
  }

  printSolution(dist, vertices_.size(), path_.vertex_ids);
  return std::optional<GraphTraverser::Path>();
}

std::vector<std::pair<VertexId, EdgeId>>
GraphTraverser::get_adjacent_with_distances(const VertexId& vertex_id) {
  std::vector<std::pair<VertexId, EdgeId>> adjacent_vertices;
  auto vertex_edges = vertices_[vertex_id].get_edge_ids();
  for (const auto& edge_id : vertex_edges) {
    if (edges_[edge_id].color == Edge::Color::Green)
      continue;
    else {
      if (edges_[edge_id].source == vertex_id)
        adjacent_vertices.push_back(
            std::make_pair(edges_[edge_id].destination, edge_id));
      else
        adjacent_vertices.push_back(
            std::make_pair(edges_[edge_id].source, edge_id));
    }
  }

  return adjacent_vertices;
}

}  // namespace uni_cpp_practice
