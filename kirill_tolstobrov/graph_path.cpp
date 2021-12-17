#include "graph_path.hpp"

namespace uni_cpp_practice {
GraphPath::Distance GraphPath::distance() const {
  return vertex_ids.size() - 1;
}
}  // namespace uni_cpp_practice
