#include <array>
#include <fstream>
#include <iostream>
#include "graph.hpp"

void writeGraphJSON(Graph* graph, const std::string filename) {
  std::ofstream json(filename);
  if (json.is_open()) {
    json << graph->toJSON();
    json.close();
    std::cout << "Graph is written to " << filename << std::endl;
  } else {
    std::cout << "Unable to open file" << std::endl;
  }
}

int main() {
  const std::array<Graph::InputEdge, 19> inputEdges = {{{0, {0, 1}},
                                                        {1, {0, 2}},
                                                        {2, {0, 3}},
                                                        {3, {1, 4}},
                                                        {4, {1, 5}},
                                                        {5, {1, 6}},
                                                        {6, {2, 7}},
                                                        {7, {2, 8}},
                                                        {8, {3, 9}},
                                                        {9, {4, 10}},
                                                        {10, {5, 10}},
                                                        {11, {6, 10}},
                                                        {12, {7, 11}},
                                                        {13, {8, 11}},
                                                        {14, {9, 12}},
                                                        {15, {10, 13}},
                                                        {16, {11, 13}},
                                                        {17, {12, 13}}}};

  Graph graph(inputEdges);
  writeGraphJSON(&graph, "graph.json");
  return 0;
}
