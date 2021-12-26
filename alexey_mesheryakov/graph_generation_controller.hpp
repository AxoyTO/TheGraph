#pragma once
#include <atomic>
#include <functional>
#include <list>
#include <mutex>
#include <optional>
#include <thread>
#include "graph_generator.hpp"

namespace uni_course_cpp {
class GraphGenerationController {
 public:
  using JobCallback = std::function<void()>;
  using GenStartedCallback = std::function<void(int i)>;
  using GenFinishedCallback = std::function<void(int i, Graph graph)>;

  class Worker {
   public:
    using GetJobCallback = std::function<std::optional<JobCallback>()>;

    enum class State { Idle, Working, ShouldTerminate };

    explicit Worker(const GetJobCallback& get_job_callback)
        : get_job_callback_(get_job_callback) {}

    void start();
    void stop();

    ~Worker();

   private:
    std::thread thread_;
    GetJobCallback get_job_callback_;
    std::atomic<State> state_ = State::Idle;
  };

  GraphGenerationController(
      int threads_count,
      int graphs_count,
      const GraphGenerator::Params& graph_generator_params);

  void generate(const GenStartedCallback& gen_started_callback,
                const GenFinishedCallback& gen_finished_callback);

 private:
  const int graphs_count_;
  const GraphGenerator graph_generator_;
  std::list<Worker> workers_;
  std::list<JobCallback> jobs_;
  std::mutex job_mutex_;
  std::mutex mutex_started_callback_;
  std::mutex mutex_finished_callback_;
};
}  // namespace uni_course_cpp
