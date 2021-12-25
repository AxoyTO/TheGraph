#include "concurrent_controller.hpp"

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
}  // namespace uni_cource_cpp