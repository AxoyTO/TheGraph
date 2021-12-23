#include <atomic>
#include <functional>
#include <list>
#include <mutex>
#include <optional>
#include <thread>
#include <vector>
#include "graph.hpp"
#include "graph_path.hpp"

namespace uni_cource_cpp {

class GraphTraversalController {
 public:
  using TraversalStartedCallback = std::function<void(int index)>;
  using TraversalFinishedCallback =
      std::function<void(int index, std::vector<GraphPath> paths)>;

  class Worker {
   public:
    using GetJobCallback =
        std::function<std::optional<std::function<void()>>()>;

    enum class State { Idle, Working, ShouldTerminate };

    explicit Worker(const GetJobCallback& get_job_callback)
        : get_job_callback_(get_job_callback) {}

    void start();
    void stop();

    ~Worker();

   private:
    std::thread thread_;
    GetJobCallback get_job_callback_;
    State state_ = State::Idle;
  };

  void traverse(const TraversalStartedCallback& traversalStartedCallback,
                const TraversalFinishedCallback& traversalFinishedCallback);

  GraphTraversalController(const std::vector<Graph>& graphs);

 private:
  std::list<Worker> workers_;
  std::list<std::function<void()>> jobs_;
  int threads_count_;
  const std::vector<Graph>& graphs_;
  std::mutex jobs_lock_;
  std::mutex traversalStartedCallback_lock_;
  std::mutex traversalFinishedCallback_lock_;
  std::atomic<int> graphs_travelled_ = 0;
};

}  // namespace uni_cource_cpp
