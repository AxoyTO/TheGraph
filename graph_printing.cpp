#include "graph_printing.hpp"
#include "graph_json_printing.hpp"
namespace uni_course_cpp {

std::string GraphPrinter::printGraph(const uni_course_cpp::Graph& graph) {
  std::stringstream out;
  out << "{"
      << "\n"
      << "depth: " << std::to_string(graph.getCurrentDepth()) << ",\n";
  out << "verticles: {amount:" << graph.getVertexes().size()
      << " distribution: [";
  for (int i = 0; i < graph.getCurrentDepth(); i++)
    out << std::to_string(graph.getVertexIdByDepth(i).size()) << ", ";
  out.seekp(-2, std::ios_base::end);
  out << "]}\n";
  out << "edges: {amount:" << graph.getEdges().size() << " distribution: [";
  out << uni_course_cpp::GraphJsonPrinter::printEdgeColor(
             Graph::Edge::Color::Gray)
      << ": "
      << std::to_string(
             graph
                 .getColorDistribution(uni_course_cpp::Graph::Edge::Color::Gray)
                 .size())
      << ",";
  out << uni_course_cpp::GraphJsonPrinter::printEdgeColor(
             Graph::Edge::Color::Green)
      << ": "
      << std::to_string(graph
                            .getColorDistribution(
                                uni_course_cpp::Graph::Edge::Color::Green)
                            .size())
      << ",";
  out << uni_course_cpp::GraphJsonPrinter::printEdgeColor(
             Graph::Edge::Color::Yellow)
      << ": "
      << std::to_string(graph
                            .getColorDistribution(
                                uni_course_cpp::Graph::Edge::Color::Yellow)
                            .size())
      << ",";
  out << uni_course_cpp::GraphJsonPrinter::printEdgeColor(
             Graph::Edge::Color::Red)
      << ": "
      << std::to_string(
             graph.getColorDistribution(uni_course_cpp::Graph::Edge::Color::Red)
                 .size())
      << "]}\n";
  out << "\n";
  return out.str();
}

}  // namespace uni_course_cpp
