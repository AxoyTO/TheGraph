#pragma once
#include <atomic>
#include <functional>
#include <list>
#include <mutex>
#include <thread>
#include "graph.hpp"
#include "graph_generator.hpp"
namespace uni_course_cpp {
class GraphGenerationController {
 public:
  using JobCallback = std::function<void()>;
  using GenStartedCallback = std::function<void(int index)>;
  using GenFinishedCallback = std::function<void(int index, Graph graph)>;

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

  GraphGenerationController(int threads_count,
                            int graphs_count,
                            GraphGenerator::Params graph_generator_params);

  void generate(const GenStartedCallback& gen_started_callback,
                const GenFinishedCallback& gen_finished_callback);

 private:
  std::list<Worker> workers_;
  std::list<JobCallback> jobs_;
  int graphs_count_;
  GraphGenerator graph_generator_;
  std::mutex start_callback_mutex_;
  std::mutex finish_callback_mutex_;
  std::mutex jobs_mutex_;
};
}  // namespace uni_course_cpp
