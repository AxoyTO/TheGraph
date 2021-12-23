#pragma once

#include "graph.hpp"
#include "graph_path.hpp"

namespace uni_course_cpp {

class GraphTraverser {
 public:
  GraphTraverser(const Graph& graph) : graph_(graph) {}

  GraphPath find_shortest_path(const VertexId& source_vertex_id,
                               const VertexId& destination_vertex_id) const;
  std::vector<GraphPath> find_all_paths() const;

 private:
  const Graph& graph_;
};

}  // namespace uni_course_cpp
