#pragma once
#include <string>
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
  std::string toString() const;
};
std::string edgeColorToString(const Edge::Color& color);
