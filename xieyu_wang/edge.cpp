#include <stdexcept>
#include "Edge.hpp"
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


const std::string edgeColorToString(Edge::Color color) {
  switch (color) {
    case Edge::Color::Gray:
      return "gray";
    case Edge::Color::Blue:
        return "blue";
    case Edge::Color::Green:
        return "green";
    case Edge::Color::Red:
        return "red";
    case Edge::Color::Yellow:
        return "yellow";
  }
    throw std::runtime_error("Failed to convert the color");
}
