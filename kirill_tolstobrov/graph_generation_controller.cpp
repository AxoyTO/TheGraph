#include <cassert>

#include "graph_generation_controller.hpp"

namespace uni_cpp_practice {

GraphGenerationController::GraphGenerationController(
    int threads_count,
    int graphs_count,
    const GraphGenerator::Params& graph_generator_params)
    : graphs_count_(graphs_count), graph_generator_(graph_generator_params) {
  for (int i = 0; i < threads_count; i++) {
    workers_.emplace_back([this]() -> std::optional<JobCallback> {
      const std::lock_guard lock(mutex_);
      if (jobs_.empty()) {
        return std::nullopt;
      }
      const auto first_job = jobs_.back();
      jobs_.pop_back();
      return first_job;
    });
  }
}

void GraphGenerationController::Worker::start() {
  assert(state_ == State::Idle &&
         "Trying to start the worker which is not in Idle state");
  state_ = State::Working;
  thread_ = std::thread([this]() {
    while (true) {
      if (state_ == State::ShouldTerminate) {
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

void GraphGenerationController::Worker::stop() {
  assert(state_ == State::Working &&
         "Trying to stop the worker which is not in Working state");
  state_ = State::ShouldTerminate;
  thread_.join();
}

GraphGenerationController::Worker::~Worker() {
  if (state_ == State::Working) {
    stop();
  }
}

void GraphGenerationController::generate(
    const GenStartedCallback& gen_started_callback,
    const GenFinishedCallback& gen_finished_callback) {
  std::atomic<int> done_jobs_number = 0;
  {
    const std::lock_guard lock(mutex_);
    for (int i = 0; i < graphs_count_; i++) {
      jobs_.emplace_back([this, &gen_started_callback, &gen_finished_callback,
                          &done_jobs_number, i]() {
        gen_started_callback(i);
        auto graph = graph_generator_.generate_random_graph();
        gen_finished_callback(i, std::move(graph));
        done_jobs_number++;
      });
    }
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

}  // namespace uni_cpp_practice.
