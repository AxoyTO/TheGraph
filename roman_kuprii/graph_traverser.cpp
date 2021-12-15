#include <atomic>
#include <cassert>
#include <functional>
#include <list>
#include <mutex>
#include <optional>
#include <queue>
#include <thread>
#include <vector>

#include "graph.hpp"
#include "graph_traverser.hpp"

namespace uni_cpp_practice {

namespace {

constexpr int UNVISITED = 0;
constexpr int VISITED = 1;
constexpr int MAX_DISTANCE = 10000;
const unsigned long MAX_WORKERS_COUNT = std::thread::hardware_concurrency();
}  // namespace

std::optional<GraphTraverser::Path> GraphTraverser::find_shortest_path(
    const Graph& graph,
    const VertexId& source_vertex_id,
    const VertexId& destination_vertex_id,
    std::mutex& graph_mutex) const {
  int vertices_number;

  {
    std::lock_guard lock(graph_mutex);
    if (!graph.is_vertex_exist(source_vertex_id) ||
        !graph.is_vertex_exist(destination_vertex_id))
      return std::nullopt;
  }

  const auto& source_vertex = [&vertices_number, &graph_mutex, &graph,
                               &source_vertex_id]() {
    std::lock_guard lock(graph_mutex);
    vertices_number = graph.get_vertices_num();
    const auto& vertex = graph.get_vertices().find(source_vertex_id)->second;
    return vertex;
  }();
  // unvisited vertices
  std::vector<VertexId> vertices(vertices_number, UNVISITED);
  vertices[source_vertex_id] = VISITED;
  // create distances
  std::vector<Distance> distance(vertices_number, MAX_DISTANCE);
  distance[source_vertex_id] = 0;
  // create queue
  std::queue<Vertex> vertices_queue;
  vertices_queue.push(source_vertex);
  // create path
  std::vector<std::vector<VertexId>> all_pathes(vertices_number);
  std::vector<VertexId> source_vector(1, source_vertex_id);
  all_pathes[source_vertex_id] = source_vector;

  while (!vertices_queue.empty()) {
    const auto current_vertex = vertices_queue.front();
    vertices_queue.pop();

    // check all outcoming edges
    for (const auto& edge_id : current_vertex.get_edges_ids()) {
      VertexId next_vertex_id;
      const auto& next_vertex = [&graph_mutex, &graph, &edge_id,
                                 &next_vertex_id]() {
        std::lock_guard lock(graph_mutex);
        const auto& edge = graph.get_edges().find(edge_id)->second;
        next_vertex_id = edge.connected_vertices.back();
        const auto& next_vertex =
            graph.get_vertices().find(next_vertex_id)->second;
        return next_vertex;
      }();
      // update distances
      if (distance[current_vertex.get_id()] + 1 < distance[next_vertex_id]) {
        vertices_queue.push(next_vertex);
        distance[next_vertex_id] = distance[current_vertex.get_id()] + 1;
        all_pathes[next_vertex_id] = all_pathes[current_vertex.get_id()];
        all_pathes[next_vertex_id].push_back(next_vertex_id);
        if (destination_vertex_id == next_vertex_id) {
          Path r_path(all_pathes[next_vertex_id], distance[next_vertex_id]);
          return r_path;
        }
      }
    }
  }

  return std::nullopt;
}

std::vector<GraphTraverser::Path> GraphTraverser::traverse_graph() {
  std::list<std::function<void()>> jobs;
  std::atomic<int> completed_jobs = 0;
  std::mutex graph_mutex;
  std::mutex path_mutex;
  auto vertex_ids = graph_.get_vertex_ids_at_depth(graph_.get_depth());
  std::vector<GraphTraverser::Path> pathes;
  pathes.reserve(vertex_ids.size());

  for (const auto& vertex_id : vertex_ids)
    jobs.emplace_back([this, &graph_ = graph_, &completed_jobs, &graph_mutex,
                       &vertex_id, &pathes, &path_mutex]() {
      auto path = find_shortest_path(graph_, 0, vertex_id, graph_mutex);
      {
        std::lock_guard lock(path_mutex);
        pathes.emplace_back(path.value());
      }
      completed_jobs++;
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

  const auto threads_number = std::min(vertex_ids.size(), MAX_WORKERS_COUNT);
  auto threads = std::vector<std::thread>();
  threads.reserve(threads_number);

  for (int i = 0; i < threads_number; ++i) {
    threads.emplace_back(worker);
  }

  while (completed_jobs != vertex_ids.size()) {
  }

  should_terminate = true;
  for (auto& thread : threads) {
    thread.join();
  }

  return pathes;
}

}  // namespace uni_cpp_practice
