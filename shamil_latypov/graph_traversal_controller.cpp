#include "graph_traversal_controller.hpp"
#include <algorithm>
#include <cassert>
#include <functional>
#include <mutex>
#include <optional>
#include <thread>
#include <vector>
#include "graph.hpp"
#include "graph_generation_controller.hpp"
#include "graph_traverser.hpp"

const int MAX_WORKERS_COUNT = std::thread::hardware_concurrency();

namespace uni_cource_cpp {

GraphTraversalController::GraphTraversalController(
    const std::vector<Graph>& graphs)
    : graphs_(graphs) {
  threads_count_ = std::min(MAX_WORKERS_COUNT, static_cast<int>(graphs.size()));
  for (int i = 0; i < threads_count_; i++) {
    workers_.emplace_back(
        [this, &jobs_ = jobs_,
         &jobs_lock_ = jobs_lock_]() -> std::optional<std::function<void()>> {
          const std::lock_guard lock(jobs_lock_);
          if (!jobs_.empty()) {
            const auto job = jobs_.front();
            jobs_.pop_front();
            return job;
          }
          return std::nullopt;
        });
  }
}

void GraphTraversalController::traverse(
    const TraversalStartedCallback& traversalStartedCallback,
    const TraversalFinishedCallback& traversalFinishedCallback) {
  for (int i = 0; i < graphs_.size(); i++) {
    jobs_.emplace_back(
        [this, &traversalStartedCallback, &traversalFinishedCallback,
         &graphs_travelled_ = graphs_travelled_, i,
         &traversalStartedCallback_lock_ = traversalStartedCallback_lock_,
         &traversalFinishedCallback_lock_ = traversalFinishedCallback_lock_]() {
          {
            const std::lock_guard lock(traversalStartedCallback_lock_);
            traversalStartedCallback(i);
          }
          GraphTraverser graph_traverser(graphs_[i]);
          const auto path = graph_traverser.find_all_paths();
          {
            const std::lock_guard lock(traversalFinishedCallback_lock_);
            traversalFinishedCallback(i, std::move(path));
          }
          graphs_travelled_++;
        });
  }

  for (auto& worker : workers_) {
    worker.start();
  }

  while (graphs_travelled_ != graphs_.size()) {
  }

  for (auto& worker : workers_) {
    worker.stop();
  }
}

void GraphTraversalController::Worker::start() {
  assert(state_ != State::Working && "Worker doesnt Idle");
  state_ = State::Working;
  thread_ = std::thread(
      [this, &state_ = state_, &get_job_callback_ = get_job_callback_]() {
        while (true) {
          if (state_ == State::ShouldTerminate) {
            state_ = State::Idle;
            return;
          }
          const auto job_optional = get_job_callback_();
          if (job_optional.has_value()) {
            const auto& job = job_optional.value();
            job();
          }
        }
      });
}

void GraphTraversalController::Worker::stop() {
  state_ = State::ShouldTerminate;
  thread_.join();
}

GraphTraversalController::Worker::~Worker() {
  if (state_ == State::Working)
    stop();
}

}  // namespace uni_cource_cpp
