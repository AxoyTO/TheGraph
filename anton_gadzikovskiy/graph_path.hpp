#pragma once

#include "graph.hpp"

namespace uni_cource_cpp {

struct GraphPath {
 public:
  using Distance = int;

  GraphPath(std::vector<Graph::VertexId> vertex_ids)
      : vertex_ids_(std::move(vertex_ids)){};

  Distance distance() const { return vertex_ids_.size() - 1; };

  const std::vector<Graph::VertexId>& get_path_vertex_ids() const {
    return vertex_ids_;
  };

 private:
  std::vector<Graph::VertexId> vertex_ids_;
};

}  // namespace uni_cource_cpp
