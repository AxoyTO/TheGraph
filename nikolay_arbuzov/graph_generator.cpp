#include "graph_generator.hpp"
#include <atomic>
#include <functional>
#include <list>
#include <mutex>
#include <random>
#include <thread>
#include <vector>
#include "graph.hpp"

namespace {

constexpr float k_green_probability = 0.1;
constexpr float k_red_probability = 0.33;
constexpr int k_max_threads_count = 4;

std::vector<uni_course_cpp::Graph::VertexId> get_unconnected_vertex_ids(
    const uni_course_cpp::Graph& graph,
    const uni_course_cpp::Graph::VertexId& vertex_id) {
  const auto& to_vertex_ids =
      graph.get_vertex_ids_on_depth(graph.get_vertex_depth(vertex_id) + 1);
  auto to_vertex_ids_no_neighbors =
      std::vector<uni_course_cpp::Graph::VertexId>();

  for (const auto& not_neighbor_vertex_id : to_vertex_ids) {
    if (!graph.is_connected(vertex_id, not_neighbor_vertex_id)) {
      to_vertex_ids_no_neighbors.push_back(not_neighbor_vertex_id);
    }
  }
  return to_vertex_ids_no_neighbors;
}

bool can_generate_vertex(float probability) {
  std::random_device random_device;
  std::mt19937 generate(random_device());
  std::bernoulli_distribution distribution(probability);
  return distribution(generate);
}

uni_course_cpp::Graph::VertexId get_random_vertex_id(
    const std::vector<uni_course_cpp::Graph::VertexId>& vertex_ids) {
  std::random_device random_device;
  std::mt19937 generator(random_device());
  std::uniform_int_distribution<> distribution(0, vertex_ids.size() - 1);
  return vertex_ids[distribution(generator)];
}

}  // namespace

namespace uni_course_cpp {

void GraphGenerator::generate_grey_edges(Graph& graph) const {
  // Job - это lambda функция,
  // которая энкапсулирует в себе генерацию однйо ветви

  graph.add_vertex();

  using JobCallback = std::function<void()>;
  auto jobs = std::list<JobCallback>();

  std::mutex jobs_mutex;
  std::atomic<bool> should_terminate = false;
  std::atomic<int> attempts_count = 0;

  // Заполняем список работ для воркеров
  for (int attempt_number = 0; attempt_number < params_.new_vertices_count();
       attempt_number++) {
    jobs.push_back([&attempts_count, &jobs_mutex, &graph, this]() {
      generate_grey_branch(graph, 0, 0, jobs_mutex);
      attempts_count++;
    });
  }

  // Создаем воркера,
  // который в бесконечном цикле проверяет,
  // есть ли работа, и выполняет её
  const auto worker = [&should_terminate, &jobs_mutex, &jobs]() {
    while (true) {
      // Проверка флага, должны ли мы остановить поток
      if (should_terminate) {
        return;
      }
      // Проверяем, есть ли для нас работа
      const auto job_optional = [&jobs,
                                 &jobs_mutex]() -> std::optional<JobCallback> {
        const std::lock_guard<std::mutex> lock(jobs_mutex);
        if ([&jobs]() { return jobs.size(); }()) {
          return [&jobs]() {
            const auto job = jobs.back();
            jobs.pop_back();
            return job;
          }();
        }
        return std::nullopt;
      }();

      if (job_optional.has_value()) {
        // Работа есть, выполняем её
        const auto& job = job_optional.value();
        job();
      }
    }
  };

  // Создаем и запускаем потоки с воркерами
  // MAX_THREADS_COUNT = 4
  const auto threads_count =
      std::min(k_max_threads_count, params_.new_vertices_count());
  auto threads = std::vector<std::thread>();
  threads.reserve(threads_count);
  // fill threads
  for (int i = 0; i < threads_count; ++i) {
    threads.push_back(std::thread(worker));
  }

  // Ждем, когда все ветви будут сгенерированы
  while (attempts_count < params_.new_vertices_count()) {
  }

  // Останавливем всех воркеров и потоки
  should_terminate = true;
  for (auto& thread : threads) {
    thread.join();
  }
}

void GraphGenerator::generate_grey_branch(
    Graph& graph,
    Graph::Depth current_depth,
    const Graph::VertexId& current_vertex_id,
    std::mutex& jobs_mutex) const {
  if (current_depth == params_.depth()) {
    return;
  }
  if (!can_generate_vertex(float(params_.depth() - current_depth) /
                           params_.depth())) {
    return;
  }

  const Graph::VertexId new_vertex_id = [&graph, &jobs_mutex,
                                         &current_vertex_id]() {
    const std::lock_guard<std::mutex> lock(jobs_mutex);
    const auto& new_vertex = graph.add_vertex();
    graph.add_edge(current_vertex_id, new_vertex.id);
    return new_vertex.id;
  }();
  // рекурсивно вызывает сам себя
  for (int attempt_number = 0; attempt_number < params_.new_vertices_count();
       ++attempt_number) {
    generate_grey_branch(graph, current_depth + 1, new_vertex_id, jobs_mutex);
  }
}

void GraphGenerator::generate_green_edges(Graph& graph,
                                          std::mutex& mutex) const {
  const auto& vertices = graph.vertices();
  for (const auto& vertex : vertices) {
    if (can_generate_vertex(k_green_probability)) {
      const std::lock_guard<std::mutex> lock(mutex);
      graph.add_edge(vertex.id, vertex.id);
    }
  }
}

void GraphGenerator::generate_yellow_edges(Graph& graph,
                                           std::mutex& mutex) const {
  const auto& depth = params_.depth();
  for (Graph::Depth current_depth = 0; current_depth < depth; ++current_depth) {
    for (const auto& vertex_id : graph.get_vertex_ids_on_depth(current_depth)) {
      const std::lock_guard<std::mutex> lock(mutex);
      const auto& to_vertex_ids = get_unconnected_vertex_ids(graph, vertex_id);
      if (to_vertex_ids.size() &&
          can_generate_vertex(
              float(1) - (float(depth - 1 - current_depth) / (depth - 1)))) {
        graph.add_edge(vertex_id, get_random_vertex_id(to_vertex_ids));
      }
    }
  }
}

void GraphGenerator::generate_red_edges(Graph& graph, std::mutex& mutex) const {
  const auto& depth = params_.depth();
  for (auto& vertex : graph.vertices()) {
    if (depth - graph.get_vertex_depth(vertex.id) >= 2) {
      const auto& to_vertex_ids =
          graph.get_vertex_ids_on_depth(graph.get_vertex_depth(vertex.id) + 2);
      if (to_vertex_ids.size() && can_generate_vertex(k_red_probability)) {
        const std::lock_guard<std::mutex> lock(mutex);
        graph.add_edge(vertex.id, get_random_vertex_id(to_vertex_ids));
      }
    }
  }
}

Graph GraphGenerator::generate() const {
  auto graph = Graph();
  std::mutex mutex;

  generate_grey_edges(graph);

  std::thread green_thread(
      [&graph, &mutex, this]() { generate_green_edges(graph, mutex); });
  std::thread yellow_thread(
      [&graph, &mutex, this]() { generate_yellow_edges(graph, mutex); });
  std::thread red_thread(
      [&graph, &mutex, this]() { generate_red_edges(graph, mutex); });

  green_thread.join();
  yellow_thread.join();
  red_thread.join();

  return graph;
}

}  // namespace uni_course_cpp
