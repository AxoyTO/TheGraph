#include "graph_json_printing.hpp"
namespace uni_course_cpp {
std::string GraphJsonPrinter::print_vertex(Graph::Vertex const& vertex) const {
  std::stringstream vertexOutput;
  vertexOutput << "\t{ \"id\": " << vertex.id << ", \"edge_ids\":[";
  auto const& connections = graph_.getConnections(vertex.id);
  for (int i = 0; i < (int)connections.size(); i++) {
    vertexOutput << connections[i];
    if (i != ((int)(connections.size() - 1)))
      vertexOutput << ", ";
  }
  vertexOutput << "]"
               << ",\"depth\":" << graph_.getDepth(vertex.id) << "}";
  return vertexOutput.str();
}

std::string GraphJsonPrinter::print_edge(Graph::Edge const& edge) const {
  std::stringstream edgeOutput;
  edgeOutput << "\t{ \"id\": " << edge.id << ", \"vertex_ids\":["
             << edge.fromVertexId << ", " << edge.toVertexId << "],\"color\":\""
             << printEdgeColor(edge.color) << "\"}";
  return edgeOutput.str();
}

std::string GraphJsonPrinter::print() const {
  std::stringstream printOutput;
  printOutput << "{\n\"vertices\": [\n";
  for (auto const& vertex : graph_.getVertexes()) {
    printOutput << print_vertex(vertex);
    printOutput << ",\n";
  }
  if (graph_.getVertexes().size() != 0)
    printOutput.seekp(-2, std::ios_base::end);
  printOutput << "\n\t],\n "
              << "\"edges\": [\n";
  for (auto const& edge : graph_.getEdges()) {
    printOutput << print_edge(edge);
    printOutput << ",\n";
  }
  if (graph_.getEdges().size() != 0)
    printOutput.seekp(-2, std::ios_base::end);
  printOutput << "\n\t]\n}\n";
  return printOutput.str();
}

std::string GraphJsonPrinter::printEdgeColor(Graph::Edge::Color const& color) {
  switch (color) {
    case Graph::Edge::Color::Green:
      return "green";
    case Graph::Edge::Color::Yellow:
      return "yellow";
    case Graph::Edge::Color::Red:
      return "red";
    case Graph::Edge::Color::Gray:
      return "gray";
    default:
      throw std::runtime_error("cannot print color");
  }
}
}  // namespace uni_course_cpp
