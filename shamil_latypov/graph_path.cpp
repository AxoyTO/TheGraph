#include "graph_path.hpp"
#include "graph.hpp"

namespace uni_cource_cpp {

GraphPath::Distance GraphPath::distance() const {
  Distance distant = 0;
  for (const auto& i : edge_ids) {
    distant += 1;
  }
  return distant;
}

}  // namespace uni_cource_cpp
