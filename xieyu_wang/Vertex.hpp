#pragma once
#include <string>
#include <vector>
class Vertex {
 public:
  int depth = 0;
  const int id = 0;
  explicit Vertex(int vId);
  void addEdgeId(int eId);
  std::string toString() const;
  bool hasEdgeId(int id) const;
  const std::vector<int>& getEdgeIds() const;

 private:
  std::vector<int> edgeIds_;
};
