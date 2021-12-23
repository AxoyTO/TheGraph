#include "graph_traversal_controller.hpp"
#include "graph.hpp"
#include "graph_traverser.hpp"

#include <cassert>
#include <functional>
#include <list>
#include <mutex>
#include <optional>
#include <random>
#include <thread>
#include <vector>

namespace {
const int MAX_WORKERS_COUNT = std::thread::hardware_concurrency();
}

namespace uni_cource_cpp {
GraphTraversalController::GraphTraversalController(
    const std::vector<Graph>& graphs)
    : graphs_(graphs) {
  const auto workers_count =
      std::min(MAX_WORKERS_COUNT, static_cast<int>(graphs.size()));

  for (int iter = 0; iter < workers_count; iter++) {
    workers_.emplace_back(
        [&jobs_ = jobs_,
         &get_job_mutex_ = get_job_mutex_]() -> std::optional<JobCallback> {
          const std::lock_guard lock(get_job_mutex_);
          if (jobs_.empty())
            return std::nullopt;

          const auto job = jobs_.front();
          jobs_.pop_front();

          return job;
        });
  }
}

void GraphTraversalController::traverse_graphs(
    const GenStartedCallback& gen_started_callback,
    const GenFinishedCallback& gen_finished_callback) {
  std::atomic<int> completed_jobs = 0;

  for (int i = 0; i < graphs_.size(); i++) {
    jobs_.emplace_back([&gen_started_callback = gen_started_callback,
                        &gen_finished_callback = gen_finished_callback, i,
                        &finish_callback_mutex_ = finish_callback_mutex_,
                        &start_callback_mutex_ = start_callback_mutex_,
                        &completed_jobs = completed_jobs,
                        &graphs_ = graphs_]() {
      {
        const std::lock_guard lock(start_callback_mutex_);
        gen_started_callback(i);
      }

      GraphTraverser graph_traverser(graphs_[i]);
      const auto paths = graph_traverser.traverse_graph();

      {
        const std::lock_guard lock(finish_callback_mutex_);
        gen_finished_callback(i, std::move(paths));
      }
      completed_jobs++;
    });
  }

  for (auto& worker : workers_) {
    worker.start();
  }

  while (completed_jobs != graphs_.size()) {
  }

  for (auto& worker : workers_) {
    worker.stop();
  }
}

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
          if (job_optional.has_value())
            job_optional.value()();
        }
      });
}

void GraphTraversalController::Worker::stop() {
  assert(state_ == State::Working && "Worker is already stopped");
  state_ = State::ShouldTerminate;

  thread_.join();
}

GraphTraversalController::Worker::~Worker() {
  if (state_ == State::Working)
    stop();
}
}  // namespace uni_cource_cpp
