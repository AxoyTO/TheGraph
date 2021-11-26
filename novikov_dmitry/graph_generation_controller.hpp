#pragma once

#include <functional>
#include <list>
#include <mutex>
#include <thread>
#include "graph_generator.hpp"

namespace uni_cpp_practice {

class GraphGenerationController {
 public:
  using JobCallback = std::function<void()>;
  using GetJobCallback = std::function<std::optional<JobCallback>()>;
  using GenStartedCallback = std::function<void(int)>;
  using GenFinishedCallback = std::function<void(int, Graph)>;

  class Worker {
   public:
    explicit Worker(const GetJobCallback& get_job_callback)
        : get_job_callback_(get_job_callback){};

    enum class State { Idle, Working, ShouldTerminate };

    void start();
    void stop();

    ~Worker();

   private:
    std::thread thread_;
    GetJobCallback get_job_callback_;
    State state_flag_ = State::Idle;
  };

  GraphGenerationController(
      int threads_count,
      int graphs_count,
      const GraphGenerator::Params& graph_generator_params);

  void generate(const GenStartedCallback& gen_started_callback,
                const GenFinishedCallback& gen_finished_callback);

  ~GraphGenerationController();

 private:
  std::list<Worker> workers_;
  std::list<JobCallback> jobs_;
  int graphs_count_;
  const GraphGenerator graph_generator_;
  std::mutex mutex_;
};

}  // namespace uni_cpp_practice
