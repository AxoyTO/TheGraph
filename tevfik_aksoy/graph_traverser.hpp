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

  void parse_shortest_path(std::vector<VertexId> path_vertices,
                           VertexId vertex_id);
  void save_shortest_paths(std::vector<int> distances,
                           int destination,
                           std::vector<int> path_vertices);

  std::vector<std::pair<std::vector<VertexId>, Distance>> get_shortest_paths()
      const;

 private:
  Path path_ = Path();
  std::vector<Vertex> vertices_;
  std::vector<Edge> edges_;
  std::vector<VertexId> shortest_path_;
  std::vector<std::pair<std::vector<VertexId>, Distance>> shortest_paths_;
};

}  // namespace uni_cpp_practice
