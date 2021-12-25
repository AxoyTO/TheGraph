#include <utility>
#include <vector>

#include "graph_path.hpp"

namespace uni_cource_cpp {
GraphPath::GraphPath(std::vector<VertexId> vertex_ids_vector,
                     std::vector<EdgeId> edge_ids_vector)
    : vertex_ids(std::move(vertex_ids_vector)),
      edge_ids(std::move(vertex_ids_vector)) {}

GraphPath::Distance GraphPath::distance() const {
  return edge_ids.size();
}
}  // namespace uni_cource_cpp
