#pragma once
#include "Graph.hpp"
class GraphGenerator {
 public:
  GraphGenerator(int maxDepth, int newVerticesNum);
  Graph generate() const;

 private:
  int maxDepth_;
  int newVerticesNum_;
  void generateGrey(Graph& graph) const;
  void generateGreen(Graph& graph) const;
  float getProbabilityYellow(int depth) const;
  float getProbabilityGray(int depth) const;
  void generateRed(Graph& graph) const;

  void generateYellow(Graph& graph) const;

  void generateBlue(Graph& graph) const;

  std::vector<int> getUnconnectedVertexIds(
      const int fromVertexId,
      const std::vector<int>& destinationLevel,
      Graph& graph) const;
};
int getRandomVertexId(const std::vector<int> vector1);
bool isLucky(float probability);
