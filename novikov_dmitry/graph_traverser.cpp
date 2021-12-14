#include "graph_traverser.hpp"
#include <atomic>
#include <cassert>
#include <climits>
#include <iostream>
#include <list>
#include <mutex>
#include <queue>
#include <thread>

namespace uni_cpp_practice {

constexpr int DISTANCE_MAX = INT_MAX;
constexpr int UNDEFINED_VERTEX_ID = -1;
const int MAX_THREADS_COUNT = std::thread::hardware_concurrency();

std::optional<GraphTraverser::Path> GraphTraverser::find_shortest_path(
    const VertexId& source_vertex_id,
    const VertexId& destination_vertex_id) const {
  assert(graph_.has_vertex(source_vertex_id) && "Vertex doesn't exists");
  assert(graph_.has_vertex(destination_vertex_id) && "Vertex doesn't exists");

  const auto vertices_number = graph_.get_vertex_map().size();

  std::vector<Distance> distance(vertices_number, DISTANCE_MAX);
  distance[source_vertex_id] = 0;

  std::vector<VertexId> paths_to_verticies(vertices_number,
                                           UNDEFINED_VERTEX_ID);

  std::queue<Vertex> vertices_queue;
  vertices_queue.push(graph_.get_vertex(source_vertex_id));

  while (!vertices_queue.empty()) {
    const auto current_vertex = vertices_queue.front();
    vertices_queue.pop();

    for (const auto& edge_id : current_vertex.get_edge_ids()) {
      const auto binded_vertex_id = [&edge_id, &graph_ = graph_,
                                     &current_vertex]() {
        const auto binded_vertices =
            graph_.get_edge(edge_id).get_binded_vertices();
        if (current_vertex.id != binded_vertices.first) {
          return binded_vertices.first;
        }
        return binded_vertices.second;
      }();

      if (distance[current_vertex.id] + 1 < distance[binded_vertex_id]) {
        vertices_queue.push(graph_.get_vertex(binded_vertex_id));
        distance[binded_vertex_id] = distance[current_vertex.id] + 1;
        paths_to_verticies[binded_vertex_id] = current_vertex.id;
      }
    }
  }
  std::vector<VertexId> result_path;
  VertexId current_vertex_id = destination_vertex_id;
  while (true) {
    result_path.emplace(result_path.begin(), current_vertex_id);
    current_vertex_id = paths_to_verticies[current_vertex_id];
    if (current_vertex_id == UNDEFINED_VERTEX_ID) {
      break;
    }
  };
  return Path(distance[destination_vertex_id], result_path);
}

std::vector<GraphTraverser::Path> GraphTraverser::find_all_paths() const {
  using JobCallback = std::function<void()>;
  std::list<JobCallback> jobs;

  std::vector<GraphTraverser::Path> paths;
  const auto& verticies_at_depth =
      graph_.get_vertices_at_depth(graph_.get_depth());
  paths.reserve(verticies_at_depth.size());

  std::atomic<int> jobs_counter = 0;
  std::mutex mutex_add;
  for (const auto& vertex_id : verticies_at_depth) {
    jobs.emplace_back([this, &mutex_add, &jobs_counter, &paths, &vertex_id]() {
      auto path = find_shortest_path(0, vertex_id);
      {
        std::lock_guard lock(mutex_add);
        paths.emplace_back(path.value());
      }
      ++jobs_counter;
    });
  }

  std::mutex mutex_jobs;
  std::atomic<bool> should_terminate = false;
  const auto worker = [&should_terminate, &mutex_jobs, &jobs]() {
    while (true) {
      // Проверка флага, должны ли мы остановить поток
      if (should_terminate) {
        return;
      }
      // Проверяем, есть ли для нас работа
      const auto job_optional = [&mutex_jobs,
                                 &jobs]() -> std::optional<JobCallback> {
        const std::lock_guard lock(mutex_jobs);
        if (jobs.empty()) {
          return std::nullopt;
        }
        const auto first_job = jobs.front();
        jobs.pop_front();
        return first_job;
      }();
      if (job_optional.has_value()) {
        // Работа есть, выполняем её
        const auto& job = job_optional.value();
        job();
      }
    }
  };

  std::vector<std::thread> threads;
  threads.reserve(MAX_THREADS_COUNT);
  for (int i = 0; i < MAX_THREADS_COUNT; ++i) {
    threads.emplace_back(worker);
  }

  while (jobs_counter < verticies_at_depth.size()) {
  }

  // Останавливем всех воркеров и потоки
  should_terminate = true;
  for (auto& thread : threads) {
    thread.join();
  }
  return paths;
}

}  // namespace uni_cpp_practice
