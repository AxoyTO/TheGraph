#ifndef TASK_04_EDGE_H
#define TASK_04_EDGE_H

#include <string>

class Edge {
 public:
  enum class Color { Gray, Green, Blue, Yellow, Red };
  const int id = 0;
  const int fromVertexId = 0;
  const int toVertexId = 0;
  const Color color = Color::Gray;
  Edge(int id, int fromVertexId, int toVertexId, Color color);
  std::string toString() const;
};
#endif  // TASK_04_EDGE_H
