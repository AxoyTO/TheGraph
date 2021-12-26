#pragma once

#include "graph.hpp"
#include "graph_path.hpp"

namespace uni_course_cpp {
public:
class GraphTraverser {
  GraphTraverser(const Graph& graph) graph_(graph) {}

  GraphPath find_shortest_path(VertexId source_vertex_id,
                               VertexId destination_vertex_id) const;

 private:
  const Graph graph_;
};
}  // namespace uni_course_cpp
