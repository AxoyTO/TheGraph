#include "GraphPrinter.hpp"
#include <stdexcept>

namespace uni_course_cpp {
std::string GraphPrinter::printGraph(const Graph& graph) {
  // vertex
  std::string strGraph = "";
  strGraph += "{\n\t\"vertices\": [\n";
  for (const auto& vertex : graph.getVertices()) {
    strGraph += printVertex(vertex) + ",\n";
  }
  strGraph.pop_back();
  strGraph.pop_back();
  strGraph += "\n\t],\n";
  // edges
  strGraph += "\t\"edges\": [\n";
  for (const auto& edge : graph.getEdges()) {
    strGraph += printEdge(edge) + ",\n";
  }
  strGraph.pop_back();
  strGraph.pop_back();
  strGraph += "\n\t]\n}\n";
  return strGraph;
}

std::string GraphPrinter::printVertex(const Vertex& vertex) {
  std::string str = "";
  str += "\t\t{\"id\":" + std::to_string(vertex.id) + ",";
  str += "\"edge_ids\": [";
  for (const auto& edgeId : vertex.getEdgeIds()) {
    str += std::to_string(edgeId) + ",";
  }
  str.pop_back();
  str += "], \"depth\": ";
  str += std::to_string(vertex.depth) + "}";
  return str;
}

std::string GraphPrinter::printEdge(const Edge& edge) {
  std::string strColor = printEdgeColor(edge.color);
  std::string strEdge = "";
  strEdge += "\t\t{\"id\": " + std::to_string(edge.id) + ",";
  strEdge += "\"vertex_ids\": [" + std::to_string(edge.fromVertexId) + "," +
             std::to_string(edge.toVertexId) + "], \"color\":\"" + strColor +
             "\" }";
  return strEdge;
}

std::string GraphPrinter::printEdgeColor(const Edge::Color& color) {
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

}  // namespace uni_course_cpp
