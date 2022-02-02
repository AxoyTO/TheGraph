#include <map>
#include <utility>

#include "graph.hpp"
#include "graph_traversal_controller.hpp"
#include "graph_traverser.hpp"

namespace uni_cource_cpp {
GraphTraversalController::GraphTraversalController(int threads_count,
                                                   std::map<int, Graph>& graphs)
    : graphs_(graphs) {
  init_workers(threads_count);
}

void GraphTraversalController::traverse(
    const TraversalStartedCallback& traversalStartedCallback,
    const TraversalFinishedCallback& traversalFinishedCallback) {
  init_jobs(traversalStartedCallback, traversalFinishedCallback);
  run_jobs();
}

void GraphTraversalController::init_jobs(
    const TraversalStartedCallback& traversalStartedCallback,
    const TraversalFinishedCallback& traversalFinishedCallback) {
  for (auto& [graph_id, graph] : graphs_) {
    jobs_.emplace([this, &traversalStartedCallback, &traversalFinishedCallback,
                   &graph_id = graph_id, &graph = graph]() {
      {
        const std::lock_guard controller_lock(controller_mutex_);
        traversalStartedCallback(graph_id);
      }
      const auto root_vertex_id = graph.get_root_vertex_id();
      const auto graph_last_depth = graph.depth();
      const auto last_depth_vertices_id =
          graph.get_vertices_at_depth(graph_last_depth);
      const auto graph_paths = GraphTraverser(graph).find_all_paths(
          root_vertex_id, last_depth_vertices_id);
      {
        const std::lock_guard controller_lock(controller_mutex_);
        traversalFinishedCallback(graph_id, std::move(graph_paths));
      }
    });
  }
}
}  // namespace uni_cource_cpp
