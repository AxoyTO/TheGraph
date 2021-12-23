#include "graph_path.hpp"
#include "graph.hpp"

namespace uni_cource_cpp {

GraphPath::Distance GraphPath::distance() const {
  return edge_ids.size();
}

}  // namespace uni_cource_cpp
