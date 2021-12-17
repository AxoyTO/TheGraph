#include <cassert>
#include <functional>
#include <thread>

#include "graph_path.hpp"
#include "graph_traversal_controller.hpp"
#include "graph_traverser.hpp"

namespace {
const int MAX_WORKERS_COUNT = std::thread::hardware_concurrency();
}

namespace uni_cpp_practice {

GraphTraversalController::GraphTraversalController(
    const std::vector<Graph>& graphs)
    : graphs_(graphs), graphs_count_(graphs.size()) {
  int threads_count = std::min(MAX_WORKERS_COUNT, graphs_count_);
  for (int i = 0; i < threads_count; i++) {
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

void GraphTraversalController::Worker::start() {
  assert(state_ == State::Idle &&
         "Trying to start the worker which is not in Idle state");
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
            const auto& job = job_optional.value();
            job();
          }
        }
      });
}

void GraphTraversalController::Worker::stop() {
  assert(state_ == State::Working &&
         "Trying to stop the worker which is not in Working state");
  state_ = State::ShouldTerminate;
  thread_.join();
}

GraphTraversalController::Worker::~Worker() {
  if (state_ == State::Working) {
    stop();
  }
}

void GraphTraversalController::traverse(
    const TraversalStartedCallback& trav_started_callback,
    const TraversalFinishedCallback& trav_finished_callback) {
  std::atomic<int> done_jobs_number = 0;
  for (int i = 0; i < graphs_count_; i++) {
    jobs_.emplace_back([&trav_started_callback, &trav_finished_callback,
                        &done_jobs_number, &mutex_ = mutex_, &graphs_ = graphs_,
                        i]() {
      {
        const std::lock_guard lock(mutex_);
        trav_started_callback(i);
      }
      GraphTraverser traveser(graphs_[i]);
      auto paths = traveser.find_all_paths();
      {
        const std::lock_guard lock(mutex_);
        trav_finished_callback(i, std::move(paths));
      }
      done_jobs_number++;
    });
  }

  for (auto& worker : workers_) {
    worker.start();
  }

  while (done_jobs_number < graphs_count_) {
  }

  for (auto& worker : workers_) {
    worker.stop();
  }
}

}  // namespace uni_cpp_practice
