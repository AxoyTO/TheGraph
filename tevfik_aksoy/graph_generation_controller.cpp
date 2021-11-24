#include "graph_generation_controller.hpp"

namespace uni_cpp_practice {
void GraphGenerationController::generate(
    const Worker::GenerateStartedCallback& gen_started_callback,
    const Worker::GenerateFinishedCallback& gen_finished_callback) {
  // ...
  for (auto& worker : workers_) {
    worker.start();
  }
  // ...
  for (int i = 0; i < graphs_count_; i++) {
    jobs_.emplace_back(
        [&gen_started_callback, &i, &gen_finished_callback, this]() {
          gen_started_callback(i);
          auto graph = graph_generator_.generate();
          gen_finished_callback(i, std::move(graph));
        });
  }
  // ...
}

void GraphGenerationController::Worker::start() {
  thread_ = std::thread([this]() {
    while (true) {
      /*if should_terminate()) {
        return;
      }*/
      const auto job_optional = get_job_callback_();
      if (job_optional.has_value()) {
        const auto job_callback = job_optional.value();
        job_callback();
      }
    }
  });
}

// void GraphGenerationController::Worker::stop() {}

}  // namespace uni_cpp_practice