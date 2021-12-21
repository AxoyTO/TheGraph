#include "graph_traversal_controller.hpp"

namespace uni_cource_cpp {

using std::lock_guard;
using std::mutex;
using std::ref;
using std::thread;
using std::vector;
using uni_cource_cpp::GraphTraversalController;
const int kMaxWorkersCount = std::thread::hardware_concurrency();

void GraphTraversalController::traverse(
    const TraversalStartedCallback& traversal_started_callback,
    const TraversalFinishedCallback& traversal_finished_callback) {
  std::atomic<int> jobs_done = 0;
  mutex traversal_started_mutex;
  mutex traversal_finished_mutex;

  for (int i = 0; i < graphs_.size(); i++) {
    jobs_.push_back([&traversal_started_mutex, &traversal_started_callback,
                     &traversal_finished_mutex, &traversal_finished_callback,
                     &jobs_done, &graphs_ = graphs_, i]() {
      {
        const lock_guard lock(traversal_started_mutex);
        traversal_started_callback(i);
      }
      GraphTraverser graph_traverser(graphs_[i]);
      const auto found_paths = graph_traverser.find_all_paths();
      {
        const lock_guard lock(traversal_finished_mutex);
        traversal_finished_callback(i, std::move(found_paths));
      }
      jobs_done++;
    });
  }

  const auto threads_count = std::min<int>(kMaxWorkersCount, graphs_.size());
  for (int i = 0; i < threads_count; i++) {
    workers_.emplace_back(
        [&jobs_ = jobs_,
         &jobs_mutex_ = jobs_mutex_]() -> std::optional<JobCallback> {
          const lock_guard lock(jobs_mutex_);
          if (!jobs_.empty()) {
            const auto next_job = jobs_.front();
            jobs_.pop_front();
            return next_job;
          }
          return std::nullopt;
        });
  }

  for (auto& worker : workers_) {
    worker.start();
  }

  while (jobs_done != graphs_.size()) {
  }

  for (auto& worker : workers_) {
    worker.stop();
  }
}

void GraphTraversalController::Worker::start() {
  state_ = State::Working;
  thread_ =
      thread([&get_job_callback_ = get_job_callback_, &state_ = state_]() {
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
  if (thread_.joinable()) {
    thread_.join();
  }
}

GraphTraversalController::Worker::~Worker() {
  if (state_ == State::Working) {
    stop();
  }
}

}  // namespace uni_cource_cpp
