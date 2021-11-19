#pragma once

#include "graph.hpp"
namespace uni_cpp_practice {
class GraphPrinter {
 public:
  explicit GraphPrinter(const Graph& graph) : graph_(graph){};

  std::string print() const;

 private:
  const Graph& graph_;
};
}  // namespace uni_cpp_practice
