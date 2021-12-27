#include "graph_printing.hpp"
#include <array>
#include <iostream>
#include <sstream>
#include "graph_json_printing.hpp"
namespace {
static const std::array<uni_course_cpp::Graph::Edge::Color, 5> colors = {
    uni_course_cpp::Graph::Edge::Color::Gray,
    uni_course_cpp::Graph::Edge::Color::Green,
    uni_course_cpp::Graph::Edge::Color::Yellow,
    uni_course_cpp::Graph::Edge::Color::Red};

}
namespace uni_course_cpp {

std::string GraphPrinter::printEdgeColor(Graph::Edge::Color const& color) {
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

std::string GraphPrinter::printGraph(const uni_course_cpp::Graph& graph) {
  std::stringstream out;
  out << "{"
      << "\n"
      << "  depth: " << std::to_string(graph.getCurrentDepth() - 1) << ",\n";
  out << "  verticles: {amount:" << graph.getVertexes().size()
      << " distribution: [";
  for (int i = 0; i < graph.getCurrentDepth(); i++)
    out << std::to_string(graph.getVertexIdByDepth(i).size()) << ", ";
  out.seekp(-2, std::ios_base::end);
  out << "]}\n";
  out << "  edges: {amount:" << graph.getEdges().size() << " distribution: [";
  for (const auto& color : colors) {
    out << printEdgeColor(color) << ": "
        << std::to_string(graph.getColorDistribution(color).size()) << ", ";
  }
  out.seekp(-2, std::ios_base::end);
  out << "]}\n}";
  out << "\n";
  return out.str();
}

}  // namespace uni_course_cpp
