#include <functional>
#include <list>
#include <mutex>
#include <optional>
#include <random>
#include <thread>
#include <vector>

#include "graph.hpp"
#include "graph_generation.hpp"
#include "graph_generation_controller.hpp"

namespace uni_cpp_practice {

namespace graph_generation_controller {

GraphGenerationController::GraphGenerationController(
    int threads_count,
    int graphs_count,
    const uni_cpp_practice::graph_generation::Params& graph_generator_params)
    : graphs_count_(graphs_count), params_(graph_generator_params) {
  for (int iter = 0; iter < threads_count; iter++) {
    workers_.emplace_back(
        [&jobs_ = jobs_, &mutex_ = mutex_]() -> std::optional<JobCallback> {
          const std::lock_guard lock(mutex_);
          if (jobs_.empty()) {
            return std::nullopt;
          }
          const auto job = jobs_.front();
          jobs_.pop_front();
          return job;
        });
  }
}

void GraphGenerationController::generate(
    const GenStartedCallback& gen_started_callback,
    const GenFinishedCallback& gen_finished_callback) {
  for (auto& worker : workers_) {
    worker.start();
  }

  for (int i = 0; i < graphs_count_; i++) {
    jobs_.emplace_back([gen_started_callback, gen_finished_callback, i,
                        &mutex_ = mutex_, &params_ = params_]() {
      gen_started_callback(i);
      auto graph = uni_cpp_practice::graph_generation::generate(params_);
      const std::lock_guard lock(mutex_);
      gen_finished_callback(std::move(graph), i);
    });
  }

  while (!jobs_.empty()) {
  }
}

GraphGenerationController::~GraphGenerationController() {
  for (auto& worker : workers_) {
    worker.stop();
  }
}

GraphGenerationController::Worker::~Worker() {
  if (thread_.joinable())
    thread_.join();
}

void GraphGenerationController::Worker::start() {
  assert(state_flag_ != State::Working);
  state_flag_ = State::Working;
  thread_ = std::thread(
      [&state_flag_ = state_flag_, &get_job_callback_ = get_job_callback_]() {
        while (true) {
          if (state_flag_ == State::ShouldTerminate) {
            return;
          }
          const auto job_optional = get_job_callback_();
          if (job_optional.has_value()) {
            job_optional.value()();
          }
        }
      });
}

void GraphGenerationController::Worker::stop() {
  assert(state_flag_ == State::Working);
  state_flag_ = State::ShouldTerminate;
}

}  // namespace graph_generation_controller

}  // namespace uni_cpp_practice
