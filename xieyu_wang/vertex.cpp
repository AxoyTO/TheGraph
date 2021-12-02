#include "Vertex.hpp"
#include <cassert>
Vertex::Vertex(int vId) : id(vId){};
void Vertex::addEdgeId(int eId) {
  assert(!hasEdgeId(eId) && "Edge Id already exists");
  edgeIds_.push_back(eId);
}
std::string Vertex::toString() const {
  std::string str = "";
  str += "\t\t{\"id\":" + std::to_string(id) + ",";
  str += "\"edge_ids\": [";
  for (const auto& edgeId : edgeIds_) {
    str += std::to_string(edgeId) + ",";
  }
  str.pop_back();
  str += "], \"depth\": ";
  str += std::to_string(depth) + "}";
  return str;
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
