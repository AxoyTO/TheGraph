#include "graph_traversal_controller.hpp"
#include <cassert>

namespace {
const int MAX_THREADS_COUNT = std::thread::hardware_concurrency();
}

namespace uni_cource_cpp {

GraphTraversalController::GraphTraversalController(
    const std::vector<Graph>& graphs)
    : graphs_(graphs) {
  const auto threads_count = std::min(MAX_THREADS_COUNT, (int)graphs_.size());
  for (int i = 0; i < threads_count; ++i) {
    workers_.emplace_back(
        [&jobs_ = jobs_,
         &job_mutex_ = job_mutex_]() -> std::optional<JobCallback> {
          const std::lock_guard lock(job_mutex_);
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
  std::atomic<int> jobs_counter = 0;
  for (int i = 0; i < graphs_.size(); ++i) {
    const auto& traversed_graph = graphs_[i];
    jobs_.emplace_back([&mutex_start_callback_ = mutex_start_callback_,
                        &mutex_finish_callback_ = mutex_finish_callback_,
                        &traversal_started_callback,
                        &traversal_finished_callback, &traversed_graph,
                        &jobs_counter = jobs_counter, i]() {
      {
        const std::lock_guard lock(mutex_start_callback_);
        traversal_started_callback(i, traversed_graph);
      }
      const auto graph_traverser = GraphTraverser(traversed_graph);
      const auto paths = graph_traverser.find_all_paths();
      {
        const std::lock_guard lock(mutex_finish_callback_);
        traversal_finished_callback(i, traversed_graph, std::move(paths));
      }
      jobs_counter++;
    });
  }

  for (auto& worker : workers_) {
    worker.start();
  }

  while (jobs_counter < graphs_.size()) {
  }

  for (auto& worker : workers_) {
    worker.stop();
  }
}

void GraphTraversalController::Worker::start() {
  assert(state_ == State::Idle && "Worker is already running");
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
            const auto job_callback = job_optional.value();
            job_callback();
          }
        }
      });
}

void GraphTraversalController::Worker::stop() {
  assert(state_ == State::Working && "Worker is not running");
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
