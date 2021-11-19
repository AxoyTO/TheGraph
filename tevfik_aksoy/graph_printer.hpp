#pragma once

#include "graph.hpp"
namespace uni_cpp_practice {
class GraphPrinter {
 public:
  explicit GraphPrinter(const Graph& _graph) : graph(_graph){};

  std::string print() const;

 private:
  const Graph& graph;
};
}  // namespace uni_cpp_practice
