#include <algorithm>
#include <cassert>

#include "graph_traversal_controller.hpp"

namespace {
const int MAX_WORKERS_COUNT = std::thread::hardware_concurrency();
}  // namespace

namespace uni_course_cpp {

using uni_course_cpp::Graph;

void GraphTraversalController::Worker::start() {
  assert(state_ != State::Working && "Worker is already working");
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
  assert(state_ == State::Working && "Worker isn't working");
  state_ = State::ShouldTerminate;
  if (thread_.joinable()) {
    thread_.join();
  }
}

GraphTraversalController::Worker::~Worker() {
  if (state_ == State::Working)
    stop();
}

GraphTraversalController::GraphTraversalController(
    const std::vector<Graph>& graphs)
    : graphs_(graphs) {
  const auto count = std::min(MAX_WORKERS_COUNT, (int)graphs_.size());
  for (int i = 0; i < count; ++i) {
    workers_.emplace_back(
        [&jobs_ = jobs_,
         &mutex_jobs_ = mutex_jobs_]() -> std::optional<JobCallback> {
          const std::lock_guard lock(mutex_jobs_);
          if (jobs_.empty()) {
            return std::nullopt;
          }
          const auto job = jobs_.front();
          jobs_.pop_front();
          return job;
        });
  }
}

void GraphTraversalController::traverse(
    const TraversalStartedCallback& traversal_started_callback,
    const TraversalFinishedCallback& traversal_finished_callback) {
  std::atomic<int> finished_jobs_count = 0;

  for (int i = 0; i < graphs_.size(); i++) {
    const auto& graph = graphs_[i];
    jobs_.emplace_back([&mutex_start_callback_ = mutex_start_callback_,
                        &mutex_finish_callback_ = mutex_finish_callback_,
                        &traversal_started_callback,
                        &traversal_finished_callback, i,
                        &finished_jobs_count = finished_jobs_count, &graph]() {
      {
        const std::lock_guard lock(mutex_start_callback_);
        traversal_started_callback(i, graph);
      }
      const auto graph_traverser = GraphTraverser(graph);
      const auto paths = graph_traverser.find_all_paths();
      {
        const std::lock_guard lock(mutex_finish_callback_);
        traversal_finished_callback(i, graph, paths);
      }
      finished_jobs_count++;
    });
  }

  for (auto& worker : workers_) {
    worker.start();
  }

  while (finished_jobs_count < graphs_.size()) {
  }

  for (auto& worker : workers_) {
    worker.stop();
  }
}

}  // namespace uni_course_cpp
