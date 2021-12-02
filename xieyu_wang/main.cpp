#include <fstream>
#include <iostream>
#include <string>
#include "GraphGenerator.hpp"
int main() {
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
  const GraphGenerator graphGenerator(maxDepth, newVerticesNum);
  const auto graph = graphGenerator.generate();
  std::ofstream writePT;
  writePT.open("Graph.json", std::ios::out);
  writePT << graph.toString() << std::endl;
  writePT.close();
  return 0;
}
