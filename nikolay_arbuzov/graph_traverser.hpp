#pragma once

#include "graph.hpp"
#include "graph_path.hpp"

namespace uni_course_cpp {
public:
class GraphTraverser {
  GraphTraverser(const Graph& graph) graph_(graph) {}

  GraphPath find_shortest_path(Graph::VertexId source_vertex_id,
                               Graph::VertexId destination_vertex_id) const;
  std::vector<GraphPath> traverse_graph();

 private:
  const Graph graph_;
};
}  // namespace uni_course_cpp
