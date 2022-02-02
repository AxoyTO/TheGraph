
#include "graph_traversal_controller.hpp"
#include <atomic>
#include <cassert>
#include <functional>
#include <list>
#include <mutex>
#include <thread>
#include <vector>
#include "graph.hpp"
#include "graph_path.hpp"
#include "graph_traverser.hpp"

namespace uni_course_cpp {

GraphTraversalController::GraphTraversalController(
    int threads_count,
    const std::vector<Graph>& graphs)
    : graphs_(graphs) {
  threads_count = std::min(threads_count, static_cast<int>(graphs.size()));
  for (int iter = 0; iter < threads_count; iter++) {
    workers_.emplace_back(
        [&jobs_ = jobs_,
         &get_job_mutex_ = get_job_mutex_]() -> std::optional<JobCallback> {
          const std::lock_guard lock(get_job_mutex_);
          if (jobs_.empty()) {
            return std::nullopt;
          }
          const auto job = jobs_.front();
          jobs_.pop_front();
          return job;
        });
  }
}

void GraphTraversalController::traverse_graphs(
    const GenStartedCallback& gen_started_callback,
    const GenFinishedCallback& gen_finished_callback) {
  std::atomic<int> jobs_done = 0;

  for (int i = 0; i < graphs_.size(); i++) {
    jobs_.emplace_back([&gen_started_callback = gen_started_callback,
                        &gen_finished_callback = gen_finished_callback, i,
                        &finish_callback_mutex_ = finish_callback_mutex_,
                        &start_callback_mutex_ = start_callback_mutex_,
                        &jobs_done = jobs_done, &graphs_ = graphs_]() {
      gen_started_callback(i);

      GraphTraverser graph_traverser(graphs_[i]);
      const auto paths = graph_traverser.traverse_graph();

      {
        const std::lock_guard lock(finish_callback_mutex_);
        gen_finished_callback(i, std::move(paths));
      }
      jobs_done++;
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

GraphTraversalController::Worker::~Worker() {
  if (state_ == State::Working)
    stop();
}

void GraphTraversalController::Worker::start() {
  assert(state_ != State::Working);
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
  assert(state_ == State::Working);
  state_ = State::ShouldTerminate;
  if (thread_.joinable())
    thread_.join();
}

}  // namespace uni_course_cpp
