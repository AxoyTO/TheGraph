#include "graph_path.hpp"
#include "graph.hpp"

namespace uni_cource_cpp {

GraphPath::Distance GraphPath::distance() const {
  Distance distant = edge_ids.size();
  return distant;
}

}  // namespace uni_cource_cpp
