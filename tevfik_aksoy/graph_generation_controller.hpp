#pragma once
#include <atomic>
#include <functional>
#include <list>
#include <mutex>
#include <thread>
#include "graph_generator.hpp"

namespace uni_cpp_practice {
class GraphGenerationController {
 public:
  using JobCallback = std::function<void()>;
  using GenerateStartedCallback = std::function<void(int)>;
  using GenerateFinishedCallback = std::function<void(int, Graph)>;

  GraphGenerationController(
      int threads_count,
      int graphs_count,
      const GraphGenerator::Params& graph_generator_params);

  class Worker {
   public:
    using GetJobCallback = std::function<std::optional<JobCallback>()>;

    explicit Worker(const GetJobCallback& get_job_callback)
        : get_job_callback_(get_job_callback) {}

    enum class State { Idle, Working, ShouldTerminate };

    void start();
    void stop();

    ~Worker();

   private:
    std::thread thread_;
    std::atomic<State> state_ = State::Idle;
    GetJobCallback get_job_callback_;
  };

  void generate(const GenerateStartedCallback& generate_started_callback,
                const GenerateFinishedCallback& generate_finished_callback);

 private:
  const int graphs_count_;
  const GraphGenerator graph_generator_;
  std::list<Worker> workers_;
  std::list<JobCallback> jobs_;
  std::mutex mutex_;
  std::mutex mutex_started_callback_;
  std::mutex mutex_finished_callback_;
};
}  // namespace uni_cpp_practice
