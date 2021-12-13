#include "graph_traversal_controller.hpp"

const int MAX_WORKERS_COUNT = std::thread::hardware_concurrency();

namespace uni_cpp_practice {
GraphTraversalController::GraphTraversalController(
    const std::vector<Graph>& graphs)
    : graphs_(graphs), graphs_count_(graphs.size()) {
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

void GraphTraversalController::traverse(
    const GraphTraversalController::TraversalStartedCallback&
        traversalStartedCallback,
    const GraphTraversalController::TraversalFinishedCallback&
        traversalFinishedCallback) {
  for (auto& worker : workers_) {
    worker.start();
  }
  std::atomic<int> jobs_count = 0;
  {
    std::lock_guard lock(mutex_);
    for (int i = 0; i < graphs_count_; i++) {
      jobs_.emplace_back(
          [&graphs = graphs_,
           &traversalStartedCallback = traversalStartedCallback,
           &traversalFinishedCallback = traversalFinishedCallback, i,
           &mutex_finished_callback = mutex_finished_callback_,
           &mutex_started_callback = mutex_started_callback_, &jobs_count]() {
            {
              const std::lock_guard lock(mutex_started_callback);
              traversalStartedCallback(i);
            }
            GraphTraversal graph_traversal(graphs[i]);
            auto path = graph_traversal.get_shortest_paths();
            {
              const std::lock_guard lock(mutex_finished_callback);
              traversalFinishedCallback(i, std::move(path));
            }
            jobs_count++;
          });
    }
  }
  while (jobs_count != graphs_count_) {
  }
  for (auto& worker : workers_) {
    worker.stop();
  }
}

void GraphTraversalController::Worker::start() {
  state_ = State::Working;
  thread_ =
      std::thread([&state_ = state_, &get_job_callback_ = get_job_callback_]() {
        while (true) {
          if (state_ == State::ShouldTerminate) {
            state_ = State::Idle;
            return;
          }
          const auto job_optional = get_job_callback_();
          if (job_optional.has_value()) {
            job_optional.value()();
          }
        }
      });
}

void GraphTraversalController::Worker::stop() {
  state_ = State::ShouldTerminate;
  if (thread_.joinable())
    thread_.join();
}
GraphTraversalController::Worker::~Worker() {
  if (state_ == State::Working)
    stop();
}

}  // namespace uni_cpp_practice
