#pragma once

#include "graph.hpp"
#include "graph_path.hpp"

namespace uni_course_cpp {

class GraphTraverser {
 public:
  GraphTraverser(const Graph& graph) : graph_(graph) {}

  GraphPath find_shortest_path(
      const Graph& graph,
      const Graph::VertexId& source_vertex_id,
      const Graph::VertexId& destination_vertex_id) const;
  std::vector<GraphPath> traverse_graph();

 private:
  const Graph graph_;
};
}  // namespace uni_course_cpp
