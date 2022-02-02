#pragma once

#include "graph.hpp"

namespace uni_course_cpp {
class GraphPath;
class GraphTraverser {
 public:
  GraphTraverser(const Graph& graph) : graph_(graph) {}

  const GraphPath find_shortest_path(
      const VertexId& source_vertex_id,
      const VertexId& destination_vertex_id) const;
  const GraphPath find_fastest_path(
      const VertexId& source_vertex_id,
      const VertexId& destination_vertex_id) const;
  std::vector<GraphPath> find_all_paths() const;

 private:
  const GraphPath find_path(const VertexId& source_vertex_id,
                            const VertexId& destination_vertex_id,
                            bool fast) const;
  const Graph& graph_;
};

}  // namespace uni_course_cpp
