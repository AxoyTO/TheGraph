#include <array>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include "GraphGenerator.hpp"
#include "GraphPrinting.hpp"
#include "Logger.hpp"
using GraphGenerator = uni_course_cpp::GraphGenerator;
using Logger = uni_course_cpp::Logger;
using Graph = uni_course_cpp::Graph;
using Edge = uni_course_cpp::Edge;
using GraphPrinting = uni_course_cpp::GraphPrinting;
int ctrlMaxDepthEntry() {
  int maxDepth = 0;
  std::cout << "Enter Max Depth:";
  std::cin >> maxDepth;
  if (maxDepth < 0) {
    while (maxDepth < 0) {
      std::cout << "Error Max Depth must be  above or equal 0: ";
      std::cin >> maxDepth;
    }
  }
  return maxDepth;
}
int ctrlNewVertexNum() {
  int newVerticesNum = 0;
  std::cout << "Enter new vertices num:";
  std::cin >> newVerticesNum;
  if (newVerticesNum < 0) {
    while (newVerticesNum < 0) {
      std::cout << "Error new Vertices number must be above or equal 0: ";
      std::cin >> newVerticesNum;
    }
  }
  return newVerticesNum;
}
int ctrlNewGraphNum() {
  int newGraphNum = 0;
  std::cout << "Enter new Graph num:";
  std::cin >> newGraphNum;
  if (newGraphNum < 0) {
    while (newGraphNum < 0) {
      std::cout << "Error new Vertices number must be above or equal 0: ";
      std::cin >> newGraphNum;
    }
  }
  return newGraphNum;
}
int main() {
  Logger& logger = Logger::getLogger();
  std::filesystem::create_directory("./temp");
  int maxDepth = ctrlMaxDepthEntry();
  int newVerticesNum = ctrlNewVertexNum();
  int newGraphNum = ctrlNewGraphNum();
  for (int i = 0; i < newGraphNum; i++) {
    GraphPrinting::logStart(logger, i);
    const GraphGenerator graphGenerator(maxDepth, newVerticesNum);
    const auto graph = graphGenerator.generate();
    GraphPrinting::logEnd(logger, graph, i);
    GraphPrinting ::writeGraphToFile(graph, i);
  }
  return 0;
}
