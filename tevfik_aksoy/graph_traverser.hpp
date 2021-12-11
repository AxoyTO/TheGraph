#pragma once
#include <list>
#include <sstream>
#include "graph.hpp"

namespace uni_cpp_practice {
class GraphTraverser {
 public:
  using Distance = int;

  struct Path {
    explicit Path(Distance _distance = 0) : distance(_distance) {}
    std::vector<VertexId> vertex_ids;
    Distance distance = 0;
  };

  GraphTraverser(const Graph& graph);

  std::vector<std::pair<VertexId, EdgeId>> get_adjacent_vertices_and_distances(
      const VertexId& vertex_id);

  std::optional<Path> find_shortest_path(VertexId source_vertex_id,
                                         VertexId destination_vertex_id);

  void save_path(std::vector<VertexId> path_vertices, VertexId vertex_id);
  void print_path(std::vector<int> dist,
                  int dest,
                  std::vector<int> path_vertices);

 private:
  Path path_ = Path();
  std::vector<Vertex> vertices_;
  std::vector<Edge> edges_;
  std::vector<VertexId> path;
  std::vector<std::pair<std::vector<VertexId>, Distance>> shortest_paths;
  std::string path_string = "";
};

}  // namespace uni_cpp_practice
