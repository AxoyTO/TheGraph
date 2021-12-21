#pragma once

#include <atomic>
#include <functional>
#include <list>
#include <mutex>
#include <optional>
#include <thread>
#include "graph_traverser.hpp"

namespace uni_cource_cpp {

class GraphTraversalController {
 public:
  using JobCallback = std::function<void()>;
  using TraversalStartedCallback = std::function<void(int)>;
  using TraversalFinishedCallback =
      std::function<void(int, std::vector<GraphPath>)>;

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
    State state_ = State::Idle;
  };

  GraphTraversalController(const std::vector<Graph>& graphs)
      : graphs_(graphs){};

  void traverse(const TraversalStartedCallback& traversal_started_callback,
                const TraversalFinishedCallback& traversal_finished_callback);

 private:
  std::list<Worker> workers_;
  std::list<JobCallback> jobs_;
  std::mutex jobs_mutex_;
  std::vector<Graph> graphs_;
};

}  // namespace uni_cource_cpp
