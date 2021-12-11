#include "graph_traverser_controller.hpp"

const int MAX_WORKERS_COUNT = std::thread::hardware_concurrency();

namespace uni_cpp_practice {
GraphTraverserController::GraphTraverserController(const Graph& graph)
    : graph_(graph), graph_traverser_(graph) {
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

void GraphTraverserController::traverse(
    const GraphTraverserController::TraverserStartedCallback&
        traversalStartedCallback,
    const GraphTraverserController::TraverserFinishedCallback&
        traversalFinishedCallback) {
  for (auto& worker : workers_) {
    worker.start();
  }
  std::atomic<int> jobs_count = 0;
  {
    const std::lock_guard lock(mutex_);
    jobs_.emplace_back([&mutex_started_callback_ = mutex_started_callback_,
                        &mutex_finished_callback_ = mutex_finished_callback_,
                        &graph = graph_, &traversalStartedCallback,
                        &traversalFinishedCallback, &jobs_count = jobs_count,
                        &graph_traverser = graph_traverser_]() {
      {
        const std::lock_guard lock(mutex_started_callback_);
        traversalStartedCallback(graph);
      }
      auto shortest_paths = graph_traverser.get_shortest_paths();
      {
        const std::lock_guard lock(mutex_finished_callback_);
        traversalFinishedCallback(std::move(graph), shortest_paths);
      }
      ++jobs_count;
    });
  }

  while (jobs_count < 1) {
  }
  for (auto& worker : workers_) {
    worker.stop();
  }
}

void GraphTraverserController::Worker::start() {}
void GraphTraverserController::Worker::stop() {}
GraphTraverserController::Worker::~Worker() {}

}  // namespace uni_cpp_practice
