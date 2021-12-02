#include "edge.hpp"
Edge::Edge(int id, int fromVertexId, int toVertexId, const Color color)
    : id(id),
      fromVertexId(fromVertexId),
      toVertexId(toVertexId),
      color(color) {}
std::string Edge::toString() const {
  std::string strColor = edgeColorToString(color);
  std::string strEdge = "";
  strEdge += "\t\t{\"id\": " + std::to_string(id) + ",";
  strEdge += "\"vertex_ids\": [" + std::to_string(fromVertexId) + "," +
             std::to_string(toVertexId) + "], \"color\":\"" + strColor + "\" }";
  return strEdge;
}
std::string edgeColorToString(Edge::Color color) {
  std::string strColor;
  switch (color) {
    case Edge::Color::Gray:
      strColor = "gray";
      break;
    case Edge::Color::Blue:
      strColor = "blue";
      break;
    case Edge::Color::Green:
      strColor = "green";
      break;
    case Edge::Color::Red:
      strColor = "red";
      break;
    case Edge::Color::Yellow:
      strColor = "yellow";
  }
  return strColor;
}