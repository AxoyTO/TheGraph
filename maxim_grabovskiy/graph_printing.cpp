#include "graph_printing.hpp"
namespace printing {
std::string printGraph(const uni_course_cpp::Graph& graph) {
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
  out << "edges: {amount:" << graph.getEdges().size() << " distribution [";
  auto const& colorDistribution = graph.getColorDistribution();
  out << "Gray: " << std::to_string(colorDistribution.Gray) << ",";
  out << "Green: " << std::to_string(colorDistribution.Green) << ",";
  out << "Yellow: " << std::to_string(colorDistribution.Yellow) << ",";
  out << "Red: " << std::to_string(colorDistribution.Red) << "]}\n";
  out << "\n";
  return out.str();
}
}  // namespace printing
