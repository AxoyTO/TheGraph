#include "graph_generator.hpp"
#include <algorithm>
#include <atomic>
#include <iostream>
#include <list>
#include <mutex>
#include <random>
#include <thread>
#include "graph.hpp"

namespace {

using uni_cource_cpp::EdgeId;
using uni_cource_cpp::Graph;
using uni_cource_cpp::VertexId;

constexpr int GREEN_EDGE_CHANCE = 10;
constexpr int BLUE_EDGE_CHANCE = 25;
constexpr int RED_EDGE_CHANCE = 33;
const int MAX_THREADS_COUNT = std::thread::hardware_concurrency();

int random_number() {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> random_num(0, 100);

  return random_num(gen);
}

VertexId get_random_vertex_id(const std::vector<VertexId>& vertices_ids) {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> random_vertex(0, vertices_ids.size() - 1);

  return vertices_ids[random_vertex(gen)];
}

void generate_green_edges(Graph& graph, std::mutex& paint_edges_block) {
  for (const auto& vertex : graph.get_vertices()) {
    if (random_number() < GREEN_EDGE_CHANCE) {
      const std::lock_guard lock(paint_edges_block);
      graph.add_edge(vertex.get_id(), vertex.get_id());
    }
  }
}

void generate_blue_edges(Graph& graph, std::mutex& paint_edges_block) {
  for (const auto& depth : graph.get_depth_map()) {
    for (auto vertex_id = depth.begin(); vertex_id != depth.end() - 1;
         vertex_id++) {
      if (random_number() < BLUE_EDGE_CHANCE) {
        const std::lock_guard lock(paint_edges_block);
        graph.add_edge(*vertex_id, *(vertex_id + 1));
      }
    }
  }
}

void generate_yellow_edges(Graph& graph, std::mutex& paint_edges_block) {
  const double yellow_edge_percent = 100.0 / (double)(graph.get_depth() - 1);
  for (auto depth = (graph.get_depth_map()).begin();
       depth != (graph.get_depth_map()).end() - 1; depth++) {
    for (const auto& vertex_id : *depth) {
      // Вектор вершин уровнем глубже, которые не являются потомком vertex_id
      std::vector<VertexId> vert_ids_depth_deeper;
      for (const auto& vertex_id2 : *(depth + 1)) {
        const bool vertices_connected = [&graph, &paint_edges_block, vertex_id,
                                         vertex_id2]() {
          std::lock_guard lock(paint_edges_block);
          return graph.vertices_connected(vertex_id, vertex_id2);
        }();
        if (!vertices_connected) {
          vert_ids_depth_deeper.push_back(vertex_id2);
        }
      }
      // Проверка, что у нас есть "свободные" вершины для желтого ребра и рандом
      // выпал удачно для генерации
      if (vert_ids_depth_deeper.size() &&
          (double)random_number() <
              yellow_edge_percent *
                  (double)(depth - graph.get_depth_map().begin())) {
        const std::lock_guard lock(paint_edges_block);
        graph.add_edge(vertex_id, get_random_vertex_id(vert_ids_depth_deeper));
      }
    }
  }
}

void generate_red_edges(Graph& graph, std::mutex& paint_edges_block) {
  for (auto depth = graph.get_depth_map().begin();
       depth != graph.get_depth_map().end() - 2; depth++) {
    for (const auto& vertex_id : *depth) {
      if (random_number() < RED_EDGE_CHANCE) {
        // Выбираем рандомом вершину 2мя уровнями глубже
        const std::lock_guard lock(paint_edges_block);
        graph.add_edge(vertex_id, get_random_vertex_id(*(depth + 2)));
      }
    }
  }
}

}  // namespace

namespace uni_cource_cpp {

void GraphGenerator::generate_grey_edge(Graph& graph,
                                        const VertexId parent_vertex_id,
                                        int parent_depth,
                                        std::mutex& lock_graph) const {
  const auto new_vertex_id = [&graph, &lock_graph, parent_vertex_id]() {
    const std::lock_guard lock(lock_graph);
    auto new_vertex_id = graph.add_vertex();
    graph.add_edge(parent_vertex_id, new_vertex_id);
    return new_vertex_id;
  }();

  if (parent_depth + 1 >= depth_) {
    return;
  }

  const double percent = 100.0 / (double)depth_;

  for (int i = 0; i < new_vertices_num_; i++) {
    if ((double)random_number() > (double)parent_depth * percent) {
      generate_grey_edge(graph, new_vertex_id, parent_depth + 1, lock_graph);
    }
  }
}

void GraphGenerator::generate_vertices(Graph& graph,
                                       const VertexId& first_vertex_id) const {
  // Job - это lambda функция,
  // которая энкапсулирует в себе генерацию однйо ветви
  using JobCallback = std::function<void()>;
  auto jobs = std::list<JobCallback>();

  std::atomic<int> jobs_done = 0;
  std::mutex lock_graph;

  // Заполняем список работ для воркеров
  for (int i = 0; i < new_vertices_num_; i++) {
    jobs.emplace_back(
        [this, &graph, first_vertex_id, &lock_graph, &jobs_done]() {
          generate_grey_edge(graph, first_vertex_id, 0, lock_graph);
          jobs_done++;
        });
  }

  // Создаем воркера,
  // который в бесконечном цикле проверяет,
  // есть ли работа, и выполняет её
  std::atomic<bool> should_terminate = false;
  std::mutex jobs_lock;

  auto worker = [&should_terminate, &jobs_lock, &jobs]() {
    while (true) {
      // Проверка флага, должны ли мы остановить поток
      if (should_terminate) {
        return;
      }
      // Проверяем, есть ли для нас работа
      const auto job_optional = [&jobs_lock,
                                 &jobs]() -> std::optional<JobCallback> {
        std::lock_guard lock(jobs_lock);
        if (!jobs.empty()) {
          const auto job = jobs.front();
          jobs.pop_front();
          return job;
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
  const auto threads_count = std::min(MAX_THREADS_COUNT, new_vertices_num_);
  auto threads = std::vector<std::thread>();
  threads.reserve(threads_count);

  for (int i = 0; i < threads_count; i++) {
    threads.emplace_back(worker);
  }
  // fill threads

  // Ждем, когда все ветви будут сгенерированы
  while (jobs_done != new_vertices_num_) {
  }

  // Останавливем всех воркеров и потоки
  should_terminate = true;
  for (auto& thread : threads) {
    thread.join();
  }
}

Graph GraphGenerator::generate_graph() const {
  auto graph = Graph();
  auto first_vertex_id = graph.add_vertex();

  generate_vertices(graph, first_vertex_id);

  std::mutex paint_edges_block;
  std::thread green_thread(generate_green_edges, std::ref(graph),
                           std::ref(paint_edges_block));
  std::thread blue_thread(generate_blue_edges, std::ref(graph),
                          std::ref(paint_edges_block));
  std::thread yellow_thread(generate_yellow_edges, std::ref(graph),
                            std::ref(paint_edges_block));
  std::thread red_thread(generate_red_edges, std::ref(graph),
                         std::ref(paint_edges_block));

  green_thread.join();
  blue_thread.join();
  yellow_thread.join();
  red_thread.join();

  return graph;
}

}  // namespace uni_cource_cpp
