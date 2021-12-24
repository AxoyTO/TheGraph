#pragma once

#include "graph.hpp"
#include "graph_path.hpp"

#include <mutex>
#include <optional>
#include <vector>

namespace uni_cource_cpp {
class GraphTraverser {
 public:
  std::vector<GraphPath> find_all_paths();

  GraphPath find_shortest_path(const Graph& graph,
                               const VertexId& source_vertex_id,
                               const VertexId& destination_vertex_id) const;

  GraphTraverser(const Graph& graph) : graph_(graph) {}

 private:
  const Graph& graph_;
};
}  // namespace uni_cource_cpp
