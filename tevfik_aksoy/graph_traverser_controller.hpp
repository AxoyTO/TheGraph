#pragma once
#include <atomic>
#include <functional>
#include <mutex>
#include <thread>
#include "graph_traverser.hpp"

namespace uni_cpp_practice {
class GraphTraverserController {
 public:
  using JobCallback = std::function<void()>;
  using TraverserStartedCallback = std::function<void(const Graph&)>;
  using TraverserFinishedCallback =
      std::function<void(const Graph&, std::vector<GraphTraverser::Path>)>;

  GraphTraverserController(const Graph& graph);

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

  void traverse(const TraverserStartedCallback& traversalStartedCallback,
                const TraverserFinishedCallback& traversalFinishedCallback);

 private:
  const GraphTraverser graph_traverser_;
  std::list<Worker> workers_;
  std::list<JobCallback> jobs_;
  std::mutex mutex_;
  std::mutex mutex_started_callback_;
  std::mutex mutex_finished_callback_;
};
}  // namespace uni_cpp_practice
