#include "graph_path.hpp"
#include "graph.hpp"

namespace uni_cource_cpp {

GraphPath::Distance GraphPath::distance() const {
  Distance distance = edge_ids.size();
  return distance;
}

}  // namespace uni_cource_cpp
