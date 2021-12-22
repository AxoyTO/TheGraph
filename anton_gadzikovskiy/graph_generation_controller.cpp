#include "graph_generation_controller.hpp"

namespace uni_cource_cpp {

using std::lock_guard;
using std::mutex;
using std::ref;
using std::thread;
using std::vector;
using uni_cource_cpp::GraphGenerationController;

void GraphGenerationController::generate(
    const GenStartedCallback& gen_started_callback,
    const GenFinishedCallback& gen_finished_callback) {
  std::atomic<int> jobs_done = 0;
  mutex gen_started_mutex;
  mutex gen_finished_mutex;
  const auto generator =
      uni_cource_cpp::GraphGenerator(graph_generator_params_);

  for (int i = 0; i < graphs_count_; i++) {
    jobs_.push_back([&gen_started_mutex, &gen_started_callback,
                     &gen_finished_mutex, &gen_finished_callback, &jobs_done,
                     &generator, i]() {
      {
        const lock_guard lock(gen_started_mutex);
        gen_started_callback(i);
      }
      const auto graph = generator.generate();
      {
        const lock_guard lock(gen_finished_mutex);
        gen_finished_callback(i, std::move(graph));
      }
      jobs_done++;
    });
  }

  const auto workers_count = std::min(threads_count_, graphs_count_);
  for (int i = 0; i < workers_count; i++) {
    workers_.emplace_back(
        [&jobs_ = jobs_,
         &jobs_mutex_ = jobs_mutex_]() -> std::optional<JobCallback> {
          const lock_guard lock(jobs_mutex_);
          if (!jobs_.empty()) {
            const auto next_job = jobs_.front();
            jobs_.pop_front();
            return next_job;
          }
          return std::nullopt;
        });
  }

  for (auto& worker : workers_) {
    worker.start();
  }

  while (jobs_done != graphs_count_) {
  }

  for (auto& worker : workers_) {
    worker.stop();
  }
}

void GraphGenerationController::Worker::start() {
  state_ = State::Working;
  thread_ =
      thread([&get_job_callback_ = get_job_callback_, &state_ = state_]() {
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
  if (thread_.joinable()) {
    thread_.join();
  }
}

GraphGenerationController::Worker::~Worker() {
  if (state_ == State::Working) {
    stop();
  }
}

}  // namespace uni_cource_cpp
