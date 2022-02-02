#pragma once

#include "graph_traverser.hpp"

#include <atomic>
#include <functional>
#include <list>
#include <mutex>
#include <optional>
#include <thread>

namespace uni_course_cpp {

class GraphTraversalController {
 public:
  using JobCallback = std::function<void()>;
  using TraversalStartedCallback = std::function<void(int, const Graph&)>;
  using TraversalFinishedCallback =
      std::function<void(int, const Graph&, std::vector<GraphPath>)>;

  class Worker {
   public:
    using GetJobCallback = std::function<std::optional<JobCallback>()>;

    enum class State { Idle, Working, ShouldTerminate };

    explicit Worker(const GetJobCallback& get_job_callback)
        : get_job_callback_(get_job_callback){};

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
  const std::vector<Graph>& graphs_;
  std::list<Worker> workers_;
  std::list<JobCallback> jobs_;
  std::mutex mutex_jobs_;
  std::mutex mutex_start_callback_;
  std::mutex mutex_finish_callback_;
};

}  // namespace uni_course_cpp
