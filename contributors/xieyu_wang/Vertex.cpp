#include "Vertex.hpp"
#include <cassert>
namespace uni_course_cpp {
Vertex::Vertex(int vId) : id(vId){};
void Vertex::addEdgeId(int eId) {
  assert(!hasEdgeId(eId) && "Edge Id already exists");
  edgeIds_.push_back(eId);
}
bool Vertex::hasEdgeId(int id) const {
  for (const auto& eId : edgeIds_) {
    if (id == eId) {
      return true;
    }
  }
  return false;
}
const std::vector<int>& Vertex::getEdgeIds() const {
  return edgeIds_;
}

}  // namespace uni_course_cpp
