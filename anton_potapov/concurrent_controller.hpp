#include <atomic>
#include <functional>
#include <list>
#include <optional>
#include <queue>
#include <thread>

namespace uni_cource_cpp {
const int MAX_THREADS_COUNT = std::thread::hardware_concurrency();

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

  void init_workers(int threads_count) {
    const auto workers_count = std::min(MAX_THREADS_COUNT, threads_count);
    for (int i = 0; i < workers_count; ++i) {
      workers_.emplace_back([this]() -> std::optional<JobCallback> {
        const std::lock_guard queue_lock(jobs_queue_mutex_);
        if (!jobs_.empty()) {
          const auto job = jobs_.front();
          jobs_.pop();
          return job;
        }
        return std::nullopt;
      });
    }
  }
};
}  // namespace uni_cource_cpp
