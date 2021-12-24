#pragma once

#include <vector>

#include "graph.hpp"

namespace uni_cpp_practice {

struct GraphPath {
  using Distance = int;

  explicit GraphPath(const std::vector<VertexId>& init_vertex_ids)
      : vertex_ids(init_vertex_ids) {}

  Distance distance() const { return vertex_ids.size() - 1; };

  std::vector<VertexId> get_vertex_ids() const { return vertex_ids; }

 private:
  std::vector<VertexId> vertex_ids;
};

}  // namespace uni_cpp_practice
