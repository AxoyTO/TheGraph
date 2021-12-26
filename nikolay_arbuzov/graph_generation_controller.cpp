#include "graph_generation_controller.hpp"
#include <atomic>
#include <cassert>
#include <iostream>
#include <mutex>
#include <thread>
#include <vector>
#include "graph.hpp"
#include "graph_generator.hpp"
namespace uni_course_cpp {
void GraphGenerationController::Worker::start() {
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
            const auto& job = job_optional.value();
            job();
          }
        }
      });
}

void GraphGenerationController::Worker::stop() {
  assert(state_ == State::Working);
  state_ = State::ShouldTerminate;
  if (thread_.joinable())
    thread_.join();
}

GraphGenerationController::Worker::~Worker() {
  if (state_ == State::Working)
    stop();
}

GraphGenerationController::GraphGenerationController(
    int threads_count,
    int graphs_count,
    GraphGenerator::Params graph_generator_params)
    : graphs_count_(graphs_count), graph_generator_(graph_generator_params) {
  int workers_count = std::min(threads_count, graphs_count);
  for (int worker_number = 0; worker_number < workers_count; worker_number++) {
    workers_.emplace_back(
        [&jobs_ = jobs_,
         &jobs_mutex_ = jobs_mutex_]() -> std::optional<JobCallback> {
          const std::lock_guard<std::mutex> lock(jobs_mutex_);
          if ([&jobs_]() { return jobs_.size(); }()) {
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
  std::atomic<int> jobs_done = 0;

  for (auto& worker : workers_) {
    worker.start();
  }
  {
    std::lock_guard<std::mutex> lock(jobs_mutex_);
    for (int graph_number = 0; graph_number < graphs_count_; graph_number++) {
      jobs_.emplace_back([&gen_started_callback = gen_started_callback,
                          &gen_finished_callback = gen_finished_callback,
                          graph_number,
                          &finish_callback_mutex_ = finish_callback_mutex_,
                          &start_callback_mutex_ = start_callback_mutex_,
                          &graph_generator_ = graph_generator_, &jobs_done]() {
        {
          const std::lock_guard lock(start_callback_mutex_);
          gen_started_callback(graph_number);
        }

        auto graph = graph_generator_.generate();

        {
          const std::lock_guard lock(finish_callback_mutex_);
          gen_finished_callback(graph_number, std::move(graph));
        }

        jobs_done++;
      });
    }
  }
  while (jobs_done < graphs_count_) {
  }

  for (auto& worker : workers_) {
    worker.stop();
  }
}
}  // namespace uni_course_cpp
