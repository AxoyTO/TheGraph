#include <functional>
#include <list>
#include <optional>
#include <random>
#include <thread>
#include <vector>

#include "graph.hpp"
#include "graph_generation.hpp"
#include "graph_generation_controller.hpp"

namespace uni_cpp_practice {

namespace graph_generation_controller {

GraphGenerationController::GraphGenerationController(
    int threads_count,
    int graphs_count,
    uni_cpp_practice::graph_generation::Params graph_generator_params) {
  params_ = graph_generator_params;
  threads_count_ = threads_count;
  graphs_count_ = graphs_count;

  for (int iter = 0; iter < threads_count; iter++) {
    workers_.emplace_back([=]() -> std::optional<JobCallback> {
      if (jobs_.empty())
        return std::nullopt;
      const auto& job = jobs_.front();
      jobs_.pop_front();
      return job;
    });
  }
}

void GraphGenerationController::new_generate(
    const GenStartedCallback& gen_started_callback,
    const GenFinishedCallback& gen_finished_callback) {
  for (auto& worker : workers_) {
    worker.start();
  }

  for (int i = 0; i < graphs_count_; i++) {
    jobs_.emplace_back([=]() {
      gen_started_callback(i);
      auto graph = uni_cpp_practice::graph_generation::generate(params_);
      gen_finished_callback(std::move(graph), i);
    });
  }
}

void GraphGenerationController::Worker::start() {
  thread_ = std::thread([=]() {
    while (true) {
      //                if (should_terminate()) {
      //                    return;
      //                }
      const auto job_optional = get_job_callback_();
      if (job_optional.has_value()) {
        const auto job_callback = job_optional.value();
        job_callback();
      }
    }
  });
  // ...
}

}  // namespace graph_generation_controller

}  // namespace uni_cpp_practice
