#include "GraphGeneratorController.hpp"
#include <cassert>
#include <list>
#include <thread>

namespace uni_course_cpp {
const int MAX_THREADS_COUNT = std::thread::hardware_concurrency();

GraphGenerationController::GraphGenerationController(
    int threads_count,
    int graphs_count,
    GraphGenerator::Params graphGeneratorParams)
    : graphsCount_(graphs_count), graphGenerator_(graphGeneratorParams) {
  const auto workers_count = std::min(MAX_THREADS_COUNT, threads_count);
  for (int i = 0; i < workers_count; ++i) {
    workers_.emplace_back([this]() -> std::optional<JobCallback> {
      const std::lock_guard queue_lock(jobsQueueMutex_);
      if (!jobs_.empty()) {
        const auto job = jobs_.front();
        jobs_.pop();
        return job;
      }
      return std::nullopt;
    });
  }
}

void GraphGenerationController::generate(
    const GenStartedCallback& gen_started_callback,
    const GenFinishedCallback& gen_finished_callback) {
  for (int i = 0; i < graphsCount_; ++i) {
    jobs_.emplace([this, &gen_started_callback, &gen_finished_callback, i]() {
      {
        const std::lock_guard controller_lock(controllerMutex_);
        gen_started_callback(i);
      }
      auto graph = graphGenerator_.generate();
      {
        const std::lock_guard controller_lock(controllerMutex_);
        gen_finished_callback(i, std::move(graph));
      }
    });
  }

  for (auto& worker : workers_) {
    worker.start();
  }

  while (true) {
    const std::lock_guard queue_lock(jobsQueueMutex_);
    if (jobs_.empty()) {
      break;
    }
  }

  for (auto& worker : workers_) {
    worker.stop();
  }
}

GraphGenerationController::Worker::Worker(
    const GetJobCallback& get_job_callback)
    : get_job_callback_(get_job_callback) {}

void GraphGenerationController::Worker::start() {
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

void GraphGenerationController::Worker::stop() {
  assert(state_ == State::Working && "worker isn't working");
  state_ = State::ShouldTerminate;
  thread_.join();
}

GraphGenerationController::Worker::~Worker() {
  if (state_ == State::Working) {
    stop();
  }
}
}  // namespace uni_course_cpp
