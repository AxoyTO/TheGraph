#include <atomic>
#include <functional>
#include <list>
#include <mutex>
#include <optional>
#include <thread>
#include "graph.hpp"
#include "graph_generator.hpp"

namespace uni_cource_cpp {

class GraphGenerationController {
 public:
  using JobCallback = std::function<void()>;
  using GenStartedCallback = std::function<void(int)>;
  using GenFinishedCallback = std::function<void(int, Graph)>;

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

  GraphGenerationController(int threads_count,
                            int graphs_count,
                            GraphGenerator::Params graph_generator_params)
      : graphs_count_(graphs_count),
        graph_generator_params_(graph_generator_params),
        threads_count_(graphs_count){};

  void generate(const GenStartedCallback& gen_started_callback,
                const GenFinishedCallback& gen_finished_callback);

 private:
  std::list<Worker> workers_;
  std::list<JobCallback> jobs_;
  const int threads_count_;
  const int graphs_count_;
  const GraphGenerator::Params graph_generator_params_;
  std::mutex jobs_mutex_;
};

}  // namespace uni_cource_cpp
