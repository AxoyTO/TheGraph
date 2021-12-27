#include <assert.h>
#include <functional>
#include <optional>
#include <thread>

#include "concurrent_controller.hpp"
#include "config.hpp"

namespace uni_cource_cpp {
ConcurrentController::Worker::Worker(const GetJobCallback& get_job_callback)
    : get_job_callback_(get_job_callback) {}

void ConcurrentController::Worker::start() {
  assert(state_ != State::Working && "tried to start working worker");
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

void ConcurrentController::Worker::stop() {
  assert(state_ == State::Working && "worker isn't working");
  state_ = State::ShouldTerminate;
  thread_.join();
}

ConcurrentController::Worker::~Worker() {
  if (state_ == State::Working) {
    stop();
  }
}

void ConcurrentController::init_workers(int threads_count) {
  const auto workers_count = std::min(config::MAX_THREADS_COUNT, threads_count);
  for (int i = 0; i < workers_count; ++i) {
    workers_.emplace_back([this]() -> std::optional<JobCallback> {
      const std::lock_guard queue_lock(jobs_queue_mutex_);
      if (!jobs_.empty()) {
        const auto job = jobs_.front();
        jobs_.pop();
        return job;
      }
      return std::nullopt;
    });
  }
}

void ConcurrentController::run_jobs() {
  for (auto& worker : workers_) {
    worker.start();
  }
  while (true) {
    const std::lock_guard queue_lock(jobs_queue_mutex_);
    if (jobs_.empty()) {
      break;
    }
  }
  for (auto& worker : workers_) {
    worker.stop();
  }
}
}  // namespace uni_cource_cpp
