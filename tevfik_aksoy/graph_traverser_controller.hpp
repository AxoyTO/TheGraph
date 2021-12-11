#pragma once
#include "graph_traverser.hpp"
#include <functional>

namespace uni_cpp_practice{
class GraphTraverserController {
  using TraverserStartedCallback = std::function<void(const Graph&)>;
  using TraverserFinishedCallback =
      std::function<void(const Graph&, std::vector<GraphTraverser::Path>)>;

  class Worker;

  GraphTraverserController(const Graph& graph);

  void traverse(
      const TraverserStartedCallback& traversalStartedCallback,
      const TraverserFinishedCallback& traversalFinishedCallback);

 private:
  const Graph graph;
};
}
