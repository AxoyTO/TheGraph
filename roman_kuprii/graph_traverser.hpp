#pragma once

#include <mutex>
#include <optional>
#include <vector>

#include "graph.hpp"

namespace uni_cpp_practice {

class Graph;

class GraphTraverser {
 public:
  using Distance = int;

  struct Path {
    Path(std::vector<VertexId> _vertex_ids, Distance _distance)
        : vertex_ids(_vertex_ids), distance(_distance) {}
    std::vector<VertexId> vertex_ids;
    Distance distance = 0;
  };

  std::vector<Path> traverse_graph();

  Path find_shortest_path(const Graph& graph,
                          const VertexId& source_vertex_id,
                          const VertexId& destination_vertex_id) const;

  GraphTraverser(const Graph& graph) : graph_(graph) {}

 private:
  const Graph& graph_;
};

}  // namespace uni_cpp_practice
