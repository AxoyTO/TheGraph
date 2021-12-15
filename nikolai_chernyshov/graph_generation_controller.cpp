#include "graph_generation_controller.hpp"
#include <cassert>

namespace uni_course_cpp {

void GraphGenerationController::Worker::start() {
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
          if (job_optional.has_value()) {
            const auto& job = job_optional.value();
            job();
          }
        }
      });
}

void GraphGenerationController::Worker::stop() {
  assert(state_ == State::Working && "Worker isn't working");
  state_ = State::ShouldTerminate;
  if (thread_.joinable()) {
    thread_.join();
  }
}

GraphGenerationController::Worker::~Worker() {
  if (state_ == State::Working)
    stop();
}

GraphGenerationController::GraphGenerationController(
    int threads_count,
    int graphs_count,
    const GraphGenerator::Params& graph_generator_params)
    : threads_count_(threads_count),
      graphs_count_(graphs_count),
      graph_generator_(graph_generator_params) {
  const auto count = std::min(threads_count_, graphs_count_);
  for (int i = 0; i < count; ++i) {
    workers_.emplace_back(
        [&jobs_ = jobs_,
         &mutex_jobs_ = mutex_jobs_]() -> std::optional<JobCallback> {
          const std::lock_guard lock(mutex_jobs_);
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
  std::atomic<int> finished_jobs_count = 0;

  for (int i = 0; i < graphs_count_; i++) {
    jobs_.emplace_back([&mutex_start_callback_ = mutex_start_callback_,
                        &mutex_finish_callback_ = mutex_finish_callback_,
                        &gen_started_callback, &gen_finished_callback, i,
                        &finished_jobs_count = finished_jobs_count,
                        &graph_generator_ = graph_generator_]() {
      {
        const std::lock_guard lock(mutex_start_callback_);
        gen_started_callback(i);
      }
      auto graph = graph_generator_.generate();
      {
        const std::lock_guard lock(mutex_finish_callback_);
        gen_finished_callback(i, std::move(graph));
      }
      finished_jobs_count++;
    });
  }

  for (auto& worker : workers_) {
    worker.start();
  }

  while (finished_jobs_count < graphs_count_) {
  }

  for (auto& worker : workers_) {
    worker.stop();
  }
}

}  // namespace uni_course_cpp
