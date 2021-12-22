#include "graph_traversal_controller.hpp"

#include <cassert>

namespace {
const int MAX_WORKERS_COUNT = std::thread::hardware_concurrency();
}

namespace uni_cource_cpp {
GraphTraversalController::GraphTraversalController(
    const std::vector<Graph>& graphs)
    : graphs_(graphs) {
  const auto workers_number = std::min(MAX_WORKERS_COUNT, (int)graphs_.size());
  for (int i = 0; i < workers_number; ++i) {
    workers_.emplace_back(
        [&jobs_ = jobs_,
         &mutex_jobs_ = mutex_jobs_]() -> std::optional<JobCallback> {
          const std::lock_guard lock(mutex_jobs_);

          if (jobs_.empty()) 
            return std::nullopt;
            
          const auto fist_job = jobs_.front();
          jobs_.pop_front();

          return fist_job;
        });
  }
}

void GraphTraversalController::traverse(
    const TraversalStartedCallback& traversal_started_callback,
    const TraversalFinishedCallback& traversal_finished_callback) {
  for (auto& worker : workers_) {
    worker.start();
  }

  std::atomic<int> jobs_counter = 0;

  {
    const std::lock_guard lock(mutex_jobs_);
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
          traversal_finished_callback(i, std::move(paths), traversed_graph);
        }
        ++jobs_counter;
      });
    }
  }

  while (jobs_counter < graphs_.size()) {
  }

  for (auto& worker : workers_) {
    worker.stop();
  }
}

void GraphTraversalController::Worker::start() {
  assert(state_ != State::Working && "Worker is not working");

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
  assert(state_ == State::Working && "Worker is working");
  state_ = State::ShouldTerminate;

  thread_.join();
}

GraphTraversalController::Worker::~Worker() {
  if (state_ == State::Working) {
    stop();
  }
}

}  // namespace uni_cource_cpp
