#pragma once

#include <functional>
#include <list>
#include <optional>
#include <thread>

namespace uni_cpp_practice {

class Graph;

namespace graph_generation_controller {

class GraphGenerationController {
 public:
  using JobCallback = std::function<void()>;
  using GetJobCallback = std::function<std::optional<JobCallback>()>;
  using GenStartedCallback = std::function<void(int)>;
  using GenFinishedCallback = std::function<void(Graph, int)>;

  class Worker {
   public:
    explicit Worker(const GetJobCallback& get_job_callback)
        : get_job_callback_(get_job_callback){};

    void start();
    void stop();

    // FIXME
    bool should_terminate() { return false; }

   private:
    std::thread thread_;
    GetJobCallback get_job_callback_;
  };

  GraphGenerationController(
      int threads_count,
      int graphs_count,
      uni_cpp_practice::graph_generation::Params graph_generator_params);

  void new_generate(const GenStartedCallback& gen_started_callback,
                    const GenFinishedCallback& gen_finished_callback);

 private:
  std::list<Worker> workers_;
  std::list<JobCallback> jobs_;
  int threads_count_;
  int graphs_count_;
  uni_cpp_practice::graph_generation::Params params_;
};

}  // namespace graph_generation_controller

}  // namespace uni_cpp_practice
