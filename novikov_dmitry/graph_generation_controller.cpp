#include "graph_generation_controller.hpp"
#include <cassert>

namespace uni_cpp_practice {

GraphGenerationController::GraphGenerationController(
    int threads_count,
    int graphs_count,
    const GraphGenerator::Params& graph_generator_params)
    : graphs_count_(graphs_count),
      graph_generator_(GraphGenerator(graph_generator_params)) {
  for (int i = 0; i < threads_count; ++i) {
    workers_.emplace_back(
        [&jobs_ = jobs_,
         &mutex_jobs_access_controller_ =
             mutex_jobs_access_controller_]() -> std::optional<JobCallback> {
          const std::lock_guard lock(
              mutex_jobs_access_controller_);  // Блокируем доступ к общему
                                               // списку работ
          if (jobs_.empty()) {
            return std::nullopt;
          }
          const auto fist_job = jobs_.front();
          jobs_.pop_front();
          return fist_job;
        });
  }
}

void GraphGenerationController::generate(
    const GenStartedCallback& gen_started_callback,
    const GenFinishedCallback& gen_finished_callback) {
  // Запускаем воркеров
  for (auto& worker : workers_) {
    worker.start();
  }
  {  // Блокируем доступ к общему списку работ
    const std::lock_guard lock(mutex_jobs_access_controller_);
    for (int i = 0; i < graphs_count_; ++i) {
      jobs_.emplace_back([&mutex_start_callback_ = mutex_start_callback_,
                          &mutex_finish_callback_ = mutex_finish_callback_,
                          &graph_generator_ = graph_generator_,
                          &gen_started_callback, &gen_finished_callback,
                          &jobs_counter_ = jobs_counter_, i]() {
        {
          const std::lock_guard lock(
              mutex_start_callback_);  // Блокируем доступ к общему файлу
                                       // логгера для старта записи графа
          gen_started_callback(i);
        }
        auto graph = graph_generator_.generate();
        {
          const std::lock_guard lock(
              mutex_finish_callback_);  // Блокируем доступ к общему файлу
                                        // логгера для завершения записи графа
          gen_finished_callback(i, std::move(graph));
        }  // работа выполнена
        ++jobs_counter_;  // увеличиваем счетчик выполненных работ
      });
    }
  }
  while (jobs_counter_ != graphs_count_ - 1) {
  }  // Ждем что все работы выполнены
  // Выключаем всех воркеров
  for (auto& worker : workers_) {
    worker.stop();
  }
}

void GraphGenerationController::Worker::start() {
  // Проверить что `Worker` ещё не был запущен
  assert(state_ != State::Working && "Worker is not working");
  state_ = State::Working;
  // Ждем появления работы
  thread_ =
      std::thread([&state_ = state_, &get_job_callback_ = get_job_callback_]() {
        while (true) {
          // Проверка флага, должны ли мы остановить поток
          if (state_ == State::ShouldTerminate) {
            state_ = State::Idle;
            return;
          }
          const auto job_optional = get_job_callback_();
          if (job_optional.has_value()) {
            const auto job_callback = job_optional.value();
            job_callback();
          }
        }
      });
}

void GraphGenerationController::Worker::stop() {
  assert(state_ == State::Working && "Worker is working");
  state_ = State::ShouldTerminate;
  if (thread_.joinable()) {
    thread_.join();
  }
}

GraphGenerationController::Worker::~Worker() {
  if (state_ == State::Working) {
    stop();
  }
}

}  // namespace uni_cpp_practice
