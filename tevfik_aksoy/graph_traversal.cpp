#include "graph_traversal.hpp"
#include <atomic>
#include <cassert>
#include <climits>
#include <functional>
#include <iostream>
#include <list>
#include <map>
#include <mutex>
#include <queue>
#include <thread>

namespace {
const int MAX_WORKERS_COUNT = std::thread::hardware_concurrency();
}  // namespace

namespace uni_cpp_practice {

GraphTraverser::GraphTraverser(const Graph& graph) : graph_(graph) {}

std::vector<GraphTraverser::Path> GraphTraverser::traverse_graph() {
  std::list<std::function<void()>> jobs;
  std::mutex path_mutex;
  std::vector<GraphTraverser::Path> paths;
  paths.reserve(graph_.get_vertices_in_depth(graph_.depth()).size());
  std::atomic<int> jobs_count =
      graph_.get_vertices_in_depth(graph_.depth()).size();

  for (const auto& vertex_id : graph_.get_vertices_in_depth(graph_.depth()))
    jobs.emplace_back([this, &jobs_count, &vertex_id, &paths, &path_mutex]() {
      auto path = find_shortest_path(0, vertex_id);
      {
        std::lock_guard lock(path_mutex);
        paths.emplace_back(path);
      }
      jobs_count--;
    });

  std::atomic<bool> should_terminate = false;
  std::mutex jobs_mutex;
  auto worker = [&should_terminate, &jobs_mutex, &jobs]() {
    while (true) {
      if (should_terminate) {
        return;
      }
      const auto job_optional =
          [&jobs_mutex, &jobs]() -> std::optional<std::function<void()>> {
        const std::lock_guard lock(jobs_mutex);
        if (jobs.empty()) {
          return std::nullopt;
        }
        const auto job = jobs.front();
        jobs.pop_front();
        return job;
      }();
      if (job_optional.has_value()) {
        const auto& job = job_optional.value();
        job();
      }
    }
  };

  const auto threads_number = MAX_WORKERS_COUNT;
  auto threads = std::vector<std::thread>();
  threads.reserve(threads_number);

  for (int i = 0; i < threads_number; i++) {
    threads.emplace_back(worker);
  }

  while (jobs_count != 0) {
  }

  should_terminate = true;
  for (auto& thread : threads) {
    thread.join();
  }

  return paths;
}

GraphTraverser::Path GraphTraverser::find_shortest_path(
    VertexId source_vertex_id,
    VertexId destination_vertex_id) {
  assert(graph_.does_vertex_exist(source_vertex_id) &&
         "Source vertex doesn't exist!");
  assert(graph_.does_vertex_exist(destination_vertex_id) &&
         "Destination vertex doesn't exist!");

  std::priority_queue<std::pair<VertexId, Distance>,
                      std::vector<std::pair<VertexId, Distance>>,
                      std::greater<std::pair<VertexId, Distance>>>
      priority_queue;

  std::map<VertexId, VertexId> closest_vertices_map;
  std::vector<VertexId> path;

  std::vector<Distance> distances(graph_.get_vertices().size(), INT_MAX);

  priority_queue.push(std::make_pair(source_vertex_id, 0));
  distances[source_vertex_id] = 0;

  while (!priority_queue.empty()) {
    VertexId closest_vertex_id = priority_queue.top().first;
    priority_queue.pop();
    for (const auto& vertex_id :
         graph_.get_adjacent_vertex_ids(closest_vertex_id)) {
      Distance distance = 1;
      if (distances[vertex_id] > distances[closest_vertex_id] + distance) {
        closest_vertices_map[vertex_id] = closest_vertex_id;
        distances[vertex_id] = distances[closest_vertex_id] + distance;
        priority_queue.push(std::make_pair(vertex_id, distances[vertex_id]));
      }
    }
  }

  auto vertex_id = destination_vertex_id;
  while (true) {
    path.push_back(vertex_id);
    vertex_id = closest_vertices_map[vertex_id];
    if (vertex_id == 0) {
      path.push_back(vertex_id);
      break;
    }
  }
  std::reverse(path.begin(), path.end());

  Path shortest_path(path, distances[destination_vertex_id]);
  return shortest_path;
}
}  // namespace uni_cpp_practice
