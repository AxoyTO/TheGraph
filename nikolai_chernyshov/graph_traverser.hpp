#pragma once

#include <mutex>
#include "graph.hpp"

namespace uni_course_cpp {

class GraphTraverser {
 public:
  using Distance = int;

  struct GraphPath {
    GraphPath(std::vector<VertexId> _vertex_ids) : vertex_ids(_vertex_ids) {}

    Distance distance() const { return vertex_ids.size() - 1; }

    std::vector<VertexId> vertex_ids;
  };

  GraphTraverser(const Graph& graph) : graph_(graph) {}

  GraphPath find_shortest_path(const VertexId& source_vertex_id,
                               const VertexId& destination_vertex_id) const;
  std::vector<GraphPath> find_all_paths() const;

 private:
  const Graph& graph_;
};

}  // namespace uni_course_cpp
