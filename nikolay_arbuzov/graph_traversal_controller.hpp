#pragma once

#include <atomic>
#include <functional>
#include "graph.hpp"
namespace uni_course_cpp {
class GraphTraversalController {
 public:
  using JobCallback = std::function<void()>;
  using GetJobCallback = std::function<std::optional<JobCallback>()>;
  using GenStartedCallback = std::function<void(int)>;
  using GenFinishedCallback =
      std::function<void(int, const std::vector<GraphTraverser::Path>&)>;

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
    std::atomic<State> state_ = State::Idle;
  };

  GraphTraversalController(int threads_count, const std::vector<Graph>& graphs);

  void traverse_graphs(const GenStartedCallback& gen_started_callback,
                       const GenFinishedCallback& gen_finished_callback);

 private:
  std::list<Worker> workers_;
  std::list<JobCallback> jobs_;
  const std::vector<Graph>& graphs_;
  std::mutex start_callback_mutex_;
  std::mutex finish_callback_mutex_;
  std::mutex get_job_mutex_;
};
}  // namespace uni_course_cpp
