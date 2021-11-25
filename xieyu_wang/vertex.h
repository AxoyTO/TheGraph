#ifndef TASK_04_VERTEX_H
#define TASK_04_VERTEX_H
#include <string>
#include <vector>
#include <assert.h>
class Vertex {
 public:
  int depth = 0;
  const int id = 0;
  explicit Vertex(int id);
  void addEdgeId(int eId);
  std::string toString() const;
  bool hasEdgeId(int id) const;
  std::vector<int> getEdgeIds() const;

 private:
  std::vector<int> edgeIds_;
};

#endif  // TASK_04_VERTEX_H
