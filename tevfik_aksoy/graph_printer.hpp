#pragma once

#include "graph.hpp"

class GraphPrinter {
 public:
  GraphPrinter(const Graph& graph) : graph(graph){};

  std::string print() const;

 private:
  const Graph& graph;
};