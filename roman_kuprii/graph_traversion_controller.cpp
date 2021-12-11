#include <functional>
#include <list>
#include <mutex>
#include <optional>
#include <random>
#include <thread>
#include <vector>

#include "graph.hpp"
#include "graph_traverser.hpp"
#include "graph_traversion_controller.hpp"

namespace uni_cpp_practice {

namespace graph_traversion_controller {

GraphTraversionController::GraphTraversionController(int threads_count,
                                                     int graphs_count)
    : graphs_count_(graphs_count) {
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

void GraphTraversionController::traverse_graphs(
    const std::vector<Graph>& graphs,
    const GenStartedCallback& gen_started_callback,
    const GenFinishedCallback& gen_finished_callback) {
  std::atomic<int> completed_jobs = 0;

  for (auto& worker : workers_) {
    worker.start();
  }

  {
    std::lock_guard lock(get_job_mutex_);
    for (int i = 0; i < graphs_count_; i++) {
      jobs_.emplace_back([&gen_started_callback = gen_started_callback,
                          &gen_finished_callback = gen_finished_callback, i,
                          &finish_callback_mutex_ = finish_callback_mutex_,
                          &start_callback_mutex_ = start_callback_mutex_,
                          &completed_jobs = completed_jobs, &graphs]() {
        {
          const std::lock_guard lock(start_callback_mutex_);
          gen_started_callback(i);
        }
        GraphTraverser graph_traverser;
        // FIXME
        std::vector<GraphTraverser::Path> pathes;
        auto path = graph_traverser.find_shortest_path(graphs[i], 0, 6);
        if (path.has_value())
          pathes.emplace_back(path.value());
        {
          const std::lock_guard lock(finish_callback_mutex_);
          gen_finished_callback(i, std::move(pathes));
        }
        completed_jobs++;
      });
    }
  }
  while (completed_jobs != graphs_count_) {
  }

  for (auto& worker : workers_) {
    worker.stop();
  }
}

GraphTraversionController::Worker::~Worker() {
  if (state_ == State::Working)
    stop();
}

void GraphTraversionController::Worker::start() {
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

void GraphTraversionController::Worker::stop() {
  assert(state_ == State::Working);
  state_ = State::ShouldTerminate;
  if (thread_.joinable())
    thread_.join();
}

}  // namespace graph_traversion_controller

}  // namespace uni_cpp_practice
