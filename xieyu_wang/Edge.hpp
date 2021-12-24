#pragma once
#include <string>
namespace uni_course_cpp {
class Edge {
 public:
  enum class Color { Gray, Green, Blue, Yellow, Red };
  const int id = 0;
  const int fromVertexId = 0;
  const int toVertexId = 0;
  const Color color = Color::Gray;
  Edge(int id,
       int fromVertexId,
       int toVertexId,
       const Color color = Color::Gray);
};
}  // namespace uni_course_cpp
