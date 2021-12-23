#include "graph_generation_controller.hpp"
#include <cassert>
#include <functional>
#include <mutex>
#include <optional>

namespace uni_cource_cpp {

GraphGenerationController::GraphGenerationController(
    int threads_count,
    int graphs_count,
    const GraphGenerator::Params& graph_generator_params)
    : threads_count_(threads_count), graphs_count_(graphs_count) {
  graph_generator_ = GraphGenerator(graph_generator_params);
  for (int i = 0; i < threads_count_; i++) {
    workers_.emplace_back(
        [this, &jobs_ = jobs_,
         &jobs_lock_ = jobs_lock_]() -> std::optional<JobCallback> {
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

void GraphGenerationController::generate(
    const GenStartedCallback& gen_started_callback,
    const GenFinishedCallback& gen_finished_callback) {
  for (int i = 0; i < graphs_count_; i++) {
    jobs_.emplace_back([this, &gen_started_callback, &gen_finished_callback,
                        &graph_generator_ = graph_generator_,
                        &graphs_generated_ = graphs_generated_, i,
                        &function1_lock_ = function1_lock_,
                        &function2_lock_ = function2_lock_]() {
      {
        const std::lock_guard lock(function1_lock_);
        gen_started_callback(i);
      }
      auto graph = graph_generator_.generate_graph();
      {
        const std::lock_guard lock(function2_lock_);
        gen_finished_callback(i, std::move(graph));
      }
      graphs_generated_++;
    });
  }

  for (auto& worker : workers_) {
    worker.start();
  }

  while (graphs_generated_ != graphs_count_) {
  }

  for (auto& worker : workers_) {
    worker.stop();
  }
}

void GraphGenerationController::Worker::start() {
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

void GraphGenerationController::Worker::stop() {
  state_ = State::ShouldTerminate;
  thread_.join();
}

GraphGenerationController::Worker::~Worker() {
  if (state_ == State::Working)
    stop();
}

}  // namespace uni_cource_cpp
