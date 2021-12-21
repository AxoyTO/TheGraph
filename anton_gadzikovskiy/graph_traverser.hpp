#pragma once

#include "graph_path.hpp"

namespace uni_cource_cpp {

class GraphTraverser {
 public:
  GraphTraverser(const Graph& graph) : graph_(graph){};

  GraphPath find_shortest_path(
      const Graph::VertexId& source_vertex_id,
      const Graph::VertexId& destination_vertex_id) const;

  std::vector<GraphPath> find_all_paths() const;

 private:
  const Graph graph_;
};

}  // namespace uni_cource_cpp
