#pragma once

#include "graph.hpp"

namespace uni_course_cpp {

struct GraphPath {
 public:
  using Distance = int;

  GraphPath(const std::vector<VertexId>& vertex_ids)
      : vertex_ids_(vertex_ids) {}

  Distance distance() const { return vertex_ids_.size() - 1; }

  const std::vector<VertexId>& get_vertex_ids() const { return vertex_ids_; }

 private:
  std::vector<VertexId> vertex_ids_;
};

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
