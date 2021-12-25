#pragma once
#include <string>
#include <vector>
namespace uni_course_cpp {
class Vertex {
 public:
  int depth = 0;
  const int id = 0;
  explicit Vertex(int vId);
  void addEdgeId(int eId);
  bool hasEdgeId(int id) const;
  const std::vector<int>& getEdgeIds() const;

 private:
  std::vector<int> edgeIds_;
};
}  // namespace uni_course_cpp
