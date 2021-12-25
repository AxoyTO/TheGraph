#include <assert.h>
#include <list>
#include <thread>

#include "graph_generation_controller.hpp"

namespace uni_cource_cpp {

GraphGenerationController::GraphGenerationController(
    int threads_count,
    int graphs_count,
    GraphGenerator::Params graph_generator_params)
    : graphs_count_(graphs_count), graph_generator_(graph_generator_params) {
  init_workers(threads_count);
}

void GraphGenerationController::generate(
    const GenStartedCallback& gen_started_callback,
    const GenFinishedCallback& gen_finished_callback) {
  init_jobs(gen_started_callback, gen_finished_callback);
  run_jobs();
}

void GraphGenerationController::init_jobs(
    const GenStartedCallback& gen_started_callback,
    const GenFinishedCallback& gen_finished_callback) {
  for (int i = 0; i < graphs_count_; ++i) {
    jobs_.emplace([this, &gen_started_callback, &gen_finished_callback, i]() {
      {
        const std::lock_guard controller_lock(controller_mutex_);
        gen_started_callback(i);
      }
      auto graph = graph_generator_.generate_graph();
      {
        const std::lock_guard controller_lock(controller_mutex_);
        gen_finished_callback(i, std::move(graph));
      }
    });
  }
}
}  // namespace uni_cource_cpp
