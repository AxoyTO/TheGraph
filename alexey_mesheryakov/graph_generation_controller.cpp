#include "graph_generation_controller.hpp"
#include <cassert>
#include <algorithm>
namespace uni_course_cpp {

GraphGenerationController::GraphGenerationController(
    int threads_count,
    int graphs_count,
    const GraphGenerator::Params& graph_generator_params)
    : graphs_count_(graphs_count), graph_generator_(graph_generator_params) {
  for (int i = 0; i < std::min(threads_count, graphs_count); ++i) {
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

void GraphGenerationController::generate(
    const GenStartedCallback& generate_started_callback,
    const GenFinishedCallback& generate_finished_callback) {
  std::atomic<int> jobs_counter = 0;

  // Заполняем список работ для воркеров
  for (int i = 0; i < graphs_count_; ++i) {
    jobs_.emplace_back([&mutex_started_callback_ = mutex_started_callback_,
                        &mutex_finished_callback_ = mutex_finished_callback_,
                        &graph_generator_ = graph_generator_,
                        &generate_started_callback, &generate_finished_callback,
                        &jobs_counter = jobs_counter, i]() {
      {
        const std::lock_guard lock(mutex_started_callback_);
        generate_started_callback(i);
      }
      auto graph = graph_generator_.generate_graph();
      {
        const std::lock_guard lock(mutex_finished_callback_);
        generate_finished_callback(i, std::move(graph));
      }
      jobs_counter++;
    });
  }

  for (auto& worker : workers_) {
    worker.start();
  }

  while (jobs_counter < graphs_count_) {
  }

  for (auto& worker : workers_) {
    worker.stop();
  }
}

void GraphGenerationController::Worker::start() {
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

void GraphGenerationController::Worker::stop() {
  assert(state_ == State::Working && "Worker is not running");
  state_ = State::ShouldTerminate;
  thread_.join();
}

GraphGenerationController::Worker::~Worker() {
  if (state_ == State::Working) {
    stop();
  }
}

}  // namespace uni_course_cpp
