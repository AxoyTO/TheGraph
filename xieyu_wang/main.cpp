#include <array>
#include <chrono>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include "GraphGenerator.hpp"
#include "Logger.hpp"
using GraphGenerator = uni_cpp_practice::GraphGenerator;
using Logger = uni_cpp_practice::Logger;
using Graph = uni_cpp_practice::Graph;
using Edge = uni_cpp_practice::Edge;
std::string getTimeByString() {
  std::time_t presentTime =
      std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
  std::stringstream TimeInString;
  TimeInString << std::put_time(std::localtime(&presentTime), "%d/%m/%Y %T");
  return TimeInString.str();
}
void logStart(Logger& logger) {
  logger.log(getTimeByString() + ": Graph 0" + ", Generation Started\n");
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
void logColors(Logger& logger, Graph graph) {
  const std::array<Edge::Color, 5> colors = {
      Edge::Color::Gray, Edge::Color::Green, Edge::Color::Blue,
      Edge::Color::Yellow, Edge::Color::Red};
  for (int i = 0; i < colors.size(); i++) {
    logger.log(uni_cpp_practice::edgeColorToString(colors[i]) + ": " +
               std::to_string(graph.getNumEdgeByColor(colors[i])));
    if (i + 1 != colors.size())
      logger.log(", ");
  }
}
void logEnd(Logger& logger, Graph graph) {
  logger.log(getTimeByString() +
             ": Graph 0"
             ", Generation Finished {  \n");
  logger.log("  depth: " + std::to_string(getMaxDepth(logger, graph)) + ",\n");
  logger.log("  vertices: " + std::to_string(graph.getVertices().size()) +
             ", [");
  logDepth(logger, graph, getMaxDepth(logger, graph));
  logger.log("],\n  edges: " + std::to_string(graph.getEdges().size()) + ", {");
  logColors(logger, graph);
  logger.log("}\n}\n");
}
Logger& setUpLogger() {
  auto& logger = Logger ::getLogger();
  logger.setFile("./temp/log.txt");
  return logger;
}
int main() {
  Logger& logger = setUpLogger();
  int maxDepth = 0, newVerticesNum = 0;
  std::cout << "Enter Max Depth:";
  std::cin >> maxDepth;
  if (maxDepth < 0) {
    while (maxDepth < 0) {
      std::cout << "Error Max Depth must be  above or equal 0: ";
      std::cin >> maxDepth;
    }
  }
  std::cout << "Enter new vertices num:";
  std::cin >> newVerticesNum;
  if (newVerticesNum < 0) {
    while (newVerticesNum < 0) {
      std::cout << "Error new Vertices number must be above or equal 0: ";
      std::cin >> newVerticesNum;
    }
  }
  [&logger] { logStart(logger); };
  const GraphGenerator graphGenerator(maxDepth, newVerticesNum);
  const auto graph = graphGenerator.generate();
  [&logger, &graph] { logEnd(logger, graph); };
  std::ofstream writePT;
  writePT.open("Graph.json", std::ios::out);
  writePT << graph.toString() << std::endl;
  writePT.close();
  return 0;
}
