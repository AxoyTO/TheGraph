#include "LoggingHelper.hpp"
#include <array>
#include <iomanip>
#include <sstream>
#include "GraphPrinter.hpp"
namespace uni_course_cpp {
std::string LoggingHelper::getTimeByString() {
  std::time_t presentTime =
      std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
  std::stringstream TimeInString;
  TimeInString << std::put_time(std::localtime(&presentTime), "%d/%m/%Y %T");
  return TimeInString.str();
}
void LoggingHelper::logStart(Logger& logger, int graphIndex) {
  logger.log(getTimeByString() + ": Graph " + std::to_string(graphIndex) +
             ", Generation Started\n");
}
int getMaxDepth(Logger& logger, Graph graph) {
  int depth = 0;
  while (true) {
    std::vector<int> vertexIds = graph.getVertexIdsAtDepth(depth);
    if (vertexIds.size() == 0) {
      break;
    } else {
      depth++;
    }
  }
  return depth - 1;
}
void logDepth(Logger& logger, Graph graph, int maxDepth) {
  for (int i = 0; i <= maxDepth; i++) {
    logger.log(std::to_string(graph.getVertexIdsAtDepth(i).size()));
    if (i != maxDepth)
      logger.log(", ");
  }
}
void LoggingHelper::logColors(Logger& logger, Graph graph) {
  const std::array<Edge::Color, 5> colors = {
      Edge::Color::Gray, Edge::Color::Green, Edge::Color::Blue,
      Edge::Color::Yellow, Edge::Color::Red};
  for (int i = 0; i < colors.size(); i++) {
    logger.log(uni_course_cpp::GraphPrinter::printEdgeColor((colors[i])) +
               ": " + std::to_string(graph.getEdgeByColor(colors[i]).size()));
    if (i + 1 != colors.size())
      logger.log(", ");
  }
}
void LoggingHelper::logEnd(Logger& logger, Graph graph, int graphIndex) {
  logger.log(getTimeByString() + ": Graph " + std::to_string(graphIndex) +
             ", Generation Finished {  \n");
  logger.log("  depth: " + std::to_string(getMaxDepth(logger, graph)) + ",\n");
  logger.log("  vertices: " + std::to_string(graph.getVertices().size()) +
             ", [");
  logDepth(logger, graph, getMaxDepth(logger, graph));
  logger.log("],\n  edges: " + std::to_string(graph.getEdges().size()) + ", {");
  logColors(logger, graph);
  logger.log("}\n}\n");
}
}  // namespace uni_course_cpp
