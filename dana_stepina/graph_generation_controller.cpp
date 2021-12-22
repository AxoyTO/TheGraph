#include "graph_generation_controller.hpp"

#include <cassert>

namespace uni_cource_cpp {

GraphGenerationController::GraphGenerationController(
    int threads_count,
    int graphs_count,
    const GraphGenerator::Params& graph_generator_params)
    : graphs_count_(graphs_count), graph_generator_(graph_generator_params) {
  const auto workers_count = std::min(threads_count, graphs_count_);

  for (int i = 0; i < workers_count; ++i) {
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

void GraphGenerationController::generate(
    const GenStartedCallback& gen_started_callback,
    const GenFinishedCallback& gen_finished_callback) {
  std::atomic<int> jobs_counter = 0;

  for (int graph_num = 0; graph_num < graphs_count_; ++graph_num) {
    jobs_.emplace_back([&mutex_start_callback_ = mutex_start_callback_,
                        &mutex_finish_callback_ = mutex_finish_callback_,
                        &graph_generator_ = graph_generator_,
                        &gen_started_callback, &gen_finished_callback,
                        &jobs_counter = jobs_counter, graph_num]() {
      {
        const std::lock_guard lock(mutex_start_callback_);
        gen_started_callback(graph_num);
      }

      auto graph = graph_generator_.generate();

      {
        const std::lock_guard lock(mutex_finish_callback_);
        gen_finished_callback(graph_num, std::move(graph));
      }

      ++jobs_counter;
    });
  }

  for (auto& worker : workers_)
    worker.start();

  while (jobs_counter < graphs_count_) {
  }

  for (auto& worker : workers_) {
    worker.stop();
  }
}

GraphGenerationController::Worker::Worker(
    const GetJobCallback& get_job_callback)
    : get_job_callback_(get_job_callback){};

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
            const auto job_callback = job_optional.value();
            job_callback();
          }
        }
      });
}

void GraphGenerationController::Worker::stop() {
  assert(state_ == State::Working && "Worker is already stopped");
  state_ = State::ShouldTerminate;

  thread_.join();
}

GraphGenerationController::Worker::~Worker() {
  if (state_ == State::Working)
    stop();
}

}  // namespace uni_cource_cpp
