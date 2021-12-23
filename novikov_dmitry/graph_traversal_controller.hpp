#pragma once

#include <atomic>
#include <functional>
#include <list>
#include <mutex>
#include <thread>
#include "graph_traverser.hpp"

namespace uni_cpp_practice {

class GraphTraversalController {
 public:
  using JobCallback = std::function<void()>;
  using GetJobCallback = std::function<std::optional<JobCallback>()>;
  using TraversalStartedCallback = std::function<void(int, const Graph&)>;
  using TraversalFinishedCallback =
      std::function<void(int, std::vector<GraphTraverser::Path>, const Graph&)>;

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

  GraphTraversalController(const std::vector<Graph>& graphs);

  void traverse(const TraversalStartedCallback& traversal_started_callback,
                const TraversalFinishedCallback& traversal_finished_callback);

 private:
  std::list<Worker> workers_;
  std::list<JobCallback> jobs_;
  const std::vector<Graph>& graphs_;
  std::mutex mutex_jobs_;
  std::mutex mutex_start_callback_;
  std::mutex mutex_finish_callback_;
};

}  // namespace uni_cpp_practice
