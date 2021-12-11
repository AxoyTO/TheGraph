#pragma once
#include <functional>
#include "graph_traverser.hpp"

namespace uni_cpp_practice {
class GraphTraverserController {
  using TraverserStartedCallback = std::function<void(const Graph&)>;
  using TraverserFinishedCallback =
      std::function<void(const Graph&, std::vector<GraphTraverser::Path>)>;

  class Worker;

  GraphTraverserController(const Graph& graph);

  void traverse(const TraverserStartedCallback& traversalStartedCallback,
                const TraverserFinishedCallback& traversalFinishedCallback);

 private:
  const Graph graph;
};
}  // namespace uni_cpp_practice
