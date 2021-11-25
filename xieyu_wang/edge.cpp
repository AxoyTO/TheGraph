#include "edge.h"
#include <string>
Edge::Edge(int id, int fromVertexId, int toVertexId, Color color = Color::Gray)
    : id(id),
      fromVertexId(fromVertexId),
      toVertexId(toVertexId),
      color(color) {}
std::string Edge::toString() const {
  std::string strColor;
  switch (color) {
    case Color::Gray:
      strColor = "gray";
      break;
    case Color::Blue:
      strColor = "blue";
      break;
    case Color::Green:
      strColor = "green";
      break;
    case Color::Red:
      strColor = "red";
      break;
    case Color::Yellow:
      strColor = "yellow";
  }
  std::string strEdge = "";
  strEdge += "\t\t{\"id\": " + std::to_string(id) + ",";
  strEdge += "\"vertex_ids\": [" + std::to_string(fromVertexId) + "," +
             std::to_string(toVertexId) + "], \"color\":\"" + strColor + "\" }";
  return strEdge;
}
