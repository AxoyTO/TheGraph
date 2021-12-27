#pragma once

#include <atomic>
#include <functional>
#include <list>
#include <optional>
#include <queue>
#include <thread>

#include "concurrent_controller.hpp"
#include "graph.hpp"
#include "graph_generator.hpp"

namespace uni_cource_cpp {
class GraphGenerationController : public ConcurrentController {
 public:
  using GenStartedCallback = std::function<void(int index)>;
  using GenFinishedCallback = std::function<void(int index, Graph graph)>;

  GraphGenerationController(int threads_count,
                            int graphs_count,
                            GraphGenerator::Params graph_generator_params);

  void generate(const GenStartedCallback& gen_started_callback,
                const GenFinishedCallback& gen_finished_callback);

 private:
  const int graphs_count_;
  GraphGenerator graph_generator_;

  void init_jobs(const GenStartedCallback& gen_started_callback,
                 const GenFinishedCallback& gen_finished_callback);
};
}  // namespace uni_cource_cpp
