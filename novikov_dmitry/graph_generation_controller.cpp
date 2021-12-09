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
        [&jobs_ = jobs_,
         &mutex_jobs_ = mutex_jobs_]() -> std::optional<JobCallback> {
          const std::lock_guard lock(mutex_jobs_);
          if (jobs_.empty()) {
            return std::nullopt;
          }
          const auto fist_job = jobs_.front();
          jobs_.pop_front();
          return fist_job;
        });
  }
}

void GraphGenerationController::generate(
    const GenStartedCallback& gen_started_callback,
    const GenFinishedCallback& gen_finished_callback) {
  for (auto& worker : workers_) {
    worker.start();
  }
  std::atomic<int> jobs_counter = 0;
  {
    const std::lock_guard lock(mutex_jobs_);
    for (int i = 0; i < graphs_count_; ++i) {
      jobs_.emplace_back([&mutex_start_callback_ = mutex_start_callback_,
                          &mutex_finish_callback_ = mutex_finish_callback_,
                          &graph_generator_ = graph_generator_,
                          &gen_started_callback, &gen_finished_callback,
                          &jobs_counter = jobs_counter, i]() {
        {
          const std::lock_guard lock(mutex_start_callback_);
          gen_started_callback(i);
        }
        auto graph = graph_generator_.generate();
        {
          const std::lock_guard lock(mutex_finish_callback_);
          gen_finished_callback(i, std::move(graph));
        }
        ++jobs_counter;
      });
    }
  }
  while (jobs_counter < graphs_count_) {
  }
  for (auto& worker : workers_) {
    worker.stop();
  }
}

void GraphGenerationController::Worker::start() {
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

void GraphGenerationController::Worker::stop() {
  assert(state_ == State::Working && "Worker is working");
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
