#include "graph_path.hpp"
#include "graph.hpp"

namespace uni_course_cpp {
GraphPath::Distance GraphPath::distance() const {
  GraphPath::Distance distance = edge_ids.size();
  return distance;
}
}  // namespace uni_course_cpp
