#pragma once

#include <functional>

#include "concurrent_controller.hpp"
#include "graph.hpp"
#include "graph_path.hpp"

namespace uni_cource_cpp {
class GraphTraversalController : public ConcurrentController {
 public:
  using TraversalStartedCallback = std::function<void(int index)>;
  using TraversalFinishedCallback =
      std::function<void(int index, std::vector<GraphPath> paths)>;

  GraphTraversalController(int threads_count, std::map<int, Graph>& graphs);

  void traverse(const TraversalStartedCallback& traversalStartedCallback,
                const TraversalFinishedCallback& traversalFinishedCallback);

 private:
  std::map<int, Graph>& graphs_;

  void init_jobs(const TraversalStartedCallback& gen_started_callback,
                 const TraversalFinishedCallback& gen_finished_callback);
};
}  // namespace uni_cource_cpp
