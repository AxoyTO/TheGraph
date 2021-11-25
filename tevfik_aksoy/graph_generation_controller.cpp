#include "graph_generation_controller.hpp"

namespace uni_cpp_practice {
GraphGenerationController::GraphGenerationController(
    int threads_count,
    int graphs_count,
    GraphGenerator::Params graph_generator_params) {
  params_ = graph_generator_params;
  graphs_count_ = graphs_count;

  for (int i = 0; i < threads_count; i++) {
    workers_.emplace_back([this]() -> std::optional<Worker::JobCallback> {
      const std::lock_guard lock(mutex_);
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
    const Worker::GenerateStartedCallback& gen_started_callback,
    const Worker::GenerateFinishedCallback& gen_finished_callback) {
  for (auto& worker : workers_) {
    worker.start();
  }

  for (int i = 0; i < graphs_count_; i++) {
    jobs_.emplace_back(
        [&gen_started_callback, this, &i, &gen_finished_callback]() {
          gen_started_callback(i);
          auto graph = graph_generator_.generate();
          const std::lock_guard lock(mutex_);
          gen_finished_callback(i, std::move(graph));
        });
  }
}

void GraphGenerationController::Worker::start() {
  thread_ = std::thread([this]() {
    while (true) {
      const auto job_optional = get_job_callback_();
      if (job_optional.has_value()) {
        const auto job_callback = job_optional.value();
        job_callback();
      } else if (terminate_flag) {
        return;
      }
    }
  });
}

void GraphGenerationController::Worker::stop() {
  const std::lock_guard lock(mutex_);
  terminate_flag = true;
  if (thread_.joinable())
    thread_.join();
}

GraphGenerationController::~GraphGenerationController() {
  for (auto& worker : workers_) {
    worker.stop();
  }
}

}  // namespace uni_cpp_practice
