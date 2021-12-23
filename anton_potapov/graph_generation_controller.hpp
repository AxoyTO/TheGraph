#include <atomic>
#include <functional>
#include <list>
#include <optional>
#include <queue>
#include <thread>

#include "graph.hpp"
#include "graph_generator.hpp"

namespace uni_cource_cpp {
class GraphGenerationController {
 public:
  using JobCallback = std::function<void()>;
  using GenStartedCallback = std::function<void(int index)>;
  using GenFinishedCallback = std::function<void(int index, Graph graph)>;

  class Worker {
   public:
    using GetJobCallback = std::function<std::optional<JobCallback>()>;

    enum class State { Idle, Working, ShouldTerminate };

    explicit Worker(const GetJobCallback& get_job_callback);
    ~Worker();

    void start();
    void stop();

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
  std::queue<JobCallback> jobs_;
  std::mutex jobs_queue_mutex_;
  const int graphs_count_;
  GraphGenerator graph_generator_;
};
}  // namespace uni_cource_cpp
