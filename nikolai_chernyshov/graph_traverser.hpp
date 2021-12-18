#pragma once

#include <mutex>
#include "graph.hpp"

namespace uni_course_cpp {

class GraphTraverser {
 public:
  using Distance = int;

  struct GraphPath {
    Distance distance() const {return vertex_ids.size() - 1}

    std::vector<VertexId> vertex_ids;
  };

  GraphTraverser(const Graph& graph) : graph_(graph) {}

  GraphPath find_shortest_path(VertexId source_vertex_id,
                               VertexId destination_vertex_id) const;
  std::vector<GraphPath> find_all_paths() const;
  
 private:
  const Graph& graph_;
};

}  // namespace uni_course_cpp
