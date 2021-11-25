#include <cstdlib>
#include <ctime>
#include <iostream>
#include <random>
#include <string>
#include <vector>
#include "graph.h"
constexpr float GREEN_EDGE_PROBABILITY = 0.10;
constexpr float BLUE_EDGE_PROBABILITY = 0.25;
constexpr float RED_EDGE_PROBABILITY = 0.33;
class Generator {
 public:
  Generator(int maxDepth, int newVerticesNum);
  Graph generateGrey(int presentDepth, int fromVertexId);
  bool vertxGenerateProbabilityController(int depth);
  void generateGreen(Graph& graph);
  Graph generate();

 private:
  int maxDepth_;
  int newVerticesNum_;
  std::default_random_engine generator_;
  bool specialEdgeGenerateProbabilityController(Edge::Color color,
                                                int depth = 0);

  void generateRed(Graph& graph);

  void generateYellow(Graph& graph);

  void generateBlue(Graph& graph);
};
