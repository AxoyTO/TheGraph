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
    workers_.emplace_back([this]() -> std::optional<JobCallback> {
      const std::lock_guard lock(
          mutex_);  // Блокируем доступ к общему списку работ
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
  // Заполняем список работы для воркеров
  for (int i = 0; i < graphs_count_; ++i) {
    jobs_.emplace_back(
        [this, &gen_started_callback, &gen_finished_callback, i]() {
          gen_started_callback(i);
          auto graph = graph_generator_.generate();
          const std::lock_guard lock(
              mutex_);  // Блокируем доступ к общему файлу логгера
          gen_finished_callback(i, std::move(graph));
        });
  }
  // Ждем что все `jobs` выполнены и, соответственно, все графы сгенерированы
  while (!jobs_.empty()) {
  }
}

void GraphGenerationController::Worker::start() {
  // Проверить что `Worker` ещё не был запущен
  assert(flag_ != 1 && "Worker is not working");
  flag_ = 1;
  // Ждем появления работы
  thread_ = std::thread([this]() {
    while (true) {
      // Проверка флага, должны ли мы остановить поток
      if (should_terminate()) {
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
  assert(flag_ == 1 && "Worker is working");
  flag_ = 2;
}

GraphGenerationController::~GraphGenerationController() {
  for (auto& worker : workers_) {
    worker.stop();
  }
}

GraphGenerationController::Worker::~Worker() {
  if (thread_.joinable())
    thread_.join();
}

}  // namespace uni_cpp_practice
