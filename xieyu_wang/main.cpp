#include <fstream>
#include <string>
#include "generator.h"
#include "graph.h"
int main() {
  int maxDepth = 0, newVerticesNum = 0;
  std::cout << "Enter Max Depth:";
  std::cin >> maxDepth;
  std::cout << "Enter new vertices num:";
  std::cin >> newVerticesNum;
  Generator generator(maxDepth, newVerticesNum);
  const auto graph = generator.generate();
  std::ofstream writePT;
  writePT.open("Graph.json", std::ios::out);
  writePT << graph.toString() << std::endl;
  writePT.close();
  return 0;
}
