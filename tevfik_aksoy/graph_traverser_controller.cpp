#include "graph_traverser_controller.hpp"

const int MAX_WORKERS_COUNT = std::thread::hardware_concurrency();

namespace uni_cpp_practice {
GraphTraverserController::GraphTraverserController(const Graph& graph)
    : graph_traverser_(graph) {
  for (int i = 0; i < MAX_WORKERS_COUNT; ++i) {
    workers_.emplace_back(
        [&jobs_ = jobs_, &mutex_ = mutex_]() -> std::optional<JobCallback> {
          const std::lock_guard lock(mutex_);
          if (jobs_.empty()) {
            return std::nullopt;
          }
          const auto first_job = jobs_.front();
          jobs_.pop_front();
          return first_job;
        });
  }
}

void GraphTraverserController::Worker::start() {}
void GraphTraverserController::Worker::stop() {}
GraphTraverserController::Worker::~Worker() {}
void GraphTraverserController::traverse(
    const GraphTraverserController::TraverserStartedCallback&
        traversalStartedCallback,
    const GraphTraverserController::TraverserFinishedCallback&
        traversalFinishedCallback) {}
}  // namespace uni_cpp_practice
