#pragma once
#include <atomic>
#include <functional>
#include <mutex>
#include <thread>
#include "graph_traversal.hpp"

namespace uni_cpp_practice {
class GraphTraversalController {
 public:
  using JobCallback = std::function<void()>;
  using TraversalStartedCallback =
      std::function<void(int /* index */, const Graph& graph /* graph */)>;
  using TraversalFinishedCallback =
      std::function<void(int /* index */,
                         const Graph& graph /* graph */,
                         std::vector<GraphTraverser::Path> /* paths */)>;

  GraphTraversalController(const std::vector<Graph>& graphs);

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

  void traverse(const TraversalStartedCallback& traversalStartedCallback,
                const TraversalFinishedCallback& traversalFinishedCallback);

 private:
  const std::vector<Graph>& graphs_;
  const int graphs_count_;
  std::list<Worker> workers_;
  std::list<JobCallback> jobs_;
  std::mutex mutex_;
  std::mutex mutex_started_callback_;
  std::mutex mutex_finished_callback_;
};
}  // namespace uni_cpp_practice
