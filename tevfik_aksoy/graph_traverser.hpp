#pragma once
#include <list>
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

  std::vector<std::pair<VertexId, EdgeId>> get_adjacent_with_distances(
      const VertexId& vertex_id);

  std::optional<Path> find_shortest_path(VertexId source_vertex_id,
                                         VertexId destination_vertex_id);

 private:
  Path path_ = Path();
  std::vector<Vertex> vertices_;
  std::vector<Edge> edges_;
  std::vector<std::vector<Distance>> shortest_paths_;
};

}  // namespace uni_cpp_practice
