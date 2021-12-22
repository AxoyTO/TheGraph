#pragma once

#include "graph.hpp"
#include "graph_traversal.hpp"

namespace uni_cpp_practice {
class GraphPrinter {
 public:
  explicit GraphPrinter(const Graph& graph) : graph_(graph){};

  std::string print() const;

  static std::string print_path(const GraphTraverser::Path& path);

 private:
  const Graph& graph_;
};
}  // namespace uni_cpp_practice
