#pragma once

#include <functional>

namespace uni_cource_cpp {

class ConcurrentController {
 public:
  using JobCallback = std::function<void()>;

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

 protected:
  std::list<Worker> workers_;
  std::queue<JobCallback> jobs_;
  std::mutex jobs_queue_mutex_;
  std::mutex controller_mutex_;

  void init_workers(int threads_count);
  void run_jobs();
};
}  // namespace uni_cource_cpp
