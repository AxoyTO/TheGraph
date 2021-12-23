#include "Edge.hpp"
#include <stdexcept>
namespace uni_course_cpp {
Edge::Edge(int id, int fromVertexId, int toVertexId, const Color color)
    : id(id),
      fromVertexId(fromVertexId),
      toVertexId(toVertexId),
      color(color) {}
}  // namespace uni_course_cpp
