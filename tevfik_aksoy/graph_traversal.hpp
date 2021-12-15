#pragma once
#include <list>
#include "graph.hpp"

namespace uni_cpp_practice {
class GraphTraverser {
 public:
  using Distance = int;

  struct Path {
    Path(std::vector<VertexId> _vertex_ids, Distance _distance = 0)
        : vertex_ids(_vertex_ids), distance(_distance) {}
    std::vector<VertexId> vertex_ids;
    Distance distance = 0;
  };

  GraphTraverser(const Graph& graph);

  std::vector<Path> traverse_graph();

  std::optional<Path> find_shortest_path(VertexId source_vertex_id,
                                         VertexId destination_vertex_id);

 private:
  const Graph graph_;
};

}  // namespace uni_cpp_practice
