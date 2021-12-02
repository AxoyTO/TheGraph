#pragma once
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <random>
#include <string>
#include <vector>
#include "graph.hpp"
constexpr float GREEN_EDGE_PROBABILITY = 0.10;
constexpr float BLUE_EDGE_PROBABILITY = 0.25;
constexpr float RED_EDGE_PROBABILITY = 0.33;
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
  bool isLucky(float probability) const;
  void generateYellow(Graph& graph) const;

  void generateBlue(Graph& graph) const;
};
