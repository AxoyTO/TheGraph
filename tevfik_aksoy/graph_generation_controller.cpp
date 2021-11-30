#include "graph_generation_controller.hpp"
#include <cassert>

namespace uni_cpp_practice {
GraphGenerationController::GraphGenerationController(
    int threads_count,
    int graphs_count,
    const GraphGenerator::Params& graph_generator_params)
    : graphs_count_(graphs_count), graph_generator_(graph_generator_params) {
  for (int i = 0; i < threads_count; ++i) {
    workers_.emplace_back(
        [&jobs_ = jobs_, &mutex_ = mutex_]() -> std::optional<JobCallback> {
          const std::lock_guard lock(mutex_);
          if (jobs_.empty()) {
            return std::nullopt;
          }
          const auto first_job = jobs_.front();
          jobs_.pop_front();
          return first_job;
        });
  }
}

void GraphGenerationController::generate(
    const GenerateStartedCallback& generate_started_callback,
    const GenerateFinishedCallback& generate_finished_callback) {
  for (auto& worker : workers_) {
    worker.start();
  }
  std::atomic<int> jobs_count = 0;
  {
    const std::lock_guard lock(mutex_);
    for (int i = 0; i < graphs_count_; ++i) {
      jobs_.emplace_back(
          [&mutex_started_callback_ = mutex_started_callback_,
           &mutex_finished_callback_ = mutex_finished_callback_,
           &graph_generator_ = graph_generator_, &generate_started_callback,
           &generate_finished_callback, &jobs_count = jobs_count, i]() {
            {
              const std::lock_guard lock(mutex_started_callback_);
              generate_started_callback(i);
            }
            auto graph = graph_generator_.generate();
            {
              const std::lock_guard lock(mutex_finished_callback_);
              generate_finished_callback(i, std::move(graph));
            }
            ++jobs_count;
          });
    }
  }
  while (jobs_count < graphs_count_) {
  }
  for (auto& worker : workers_) {
    worker.stop();
  }
}

void GraphGenerationController::Worker::start() {
  assert(state_ == State::Idle && "Worker is not in idle state!");
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

void GraphGenerationController::Worker::stop() {
  assert(state_ == State::Working && "Worker is already stopped!");
  state_ = State::ShouldTerminate;
  if (thread_.joinable()) {
    thread_.join();
  }
}

GraphGenerationController::Worker::~Worker() {
  if (state_ == State::Working) {
    stop();
  }
}

}  // namespace uni_cpp_practice
