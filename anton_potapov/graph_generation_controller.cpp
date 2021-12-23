#include <list>

#include "graph_generation_controller.hpp"

namespace uni_cource_cpp {
GraphGenerationController::GraphGenerationController(
    int threads_count,
    int graphs_count,
    GraphGenerator::Params graph_generator_params) {}
void GraphGenerationController::generate(
    const GenStartedCallback& gen_started_callback,
    const GenFinishedCallback& gen_finished_callback) {
  // Заполняем список работ для воркеров
  for (int i = 0; i < graphs_count_; i++) {
    jobs_.emplace_back([...]() {
      gen_started_callback(i);
      auto graph = graph_generator_.generate();
      gen_finished_callback(i, std::move(graph));
    });
  }

  // Запускаем воркеров
  for (auto& worker : workers_) {
    worker.start();
  }

  // Ждем, что все `jobs` выполнены, и, соответственно, все графы сгенерированы

  // Останавливаем воркеров
}

void GraphGenerationController::Worker::start() {
  // Проверить что `Worker` ещё не был запущен

  // Создаем поток с бесконечным циклом
  // Ждем появления работы
  thread_ = std::thread([...]() {
              while (true) {
                // Проверка, должны ли мы остановить поток
                if (state_ == State::ShouldTerminate) {
                    return;
                }
                // Проверяем, есть ли для нас работа
                const auto job_optional = get_job_callback();
                if (job_optional.has_value()) {
                    // Работа есть, выполняем её
                    const auto& job = job_optional.value();
                    job();
                }
              }
            });
}

void GraphGenerationController::Worker::stop() {
  // Проверить что `Worker` работает
  // Остановить и завершить поток
  thread_.join();
}

GraphGenerationController::Worker::~Worker() {
  // При удалении мы обязательно должны остановить поток, если он запущен
  stop();
}
}  // namespace uni_cource_cpp
