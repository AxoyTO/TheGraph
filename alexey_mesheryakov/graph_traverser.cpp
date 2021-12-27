#include "graph_traverser.hpp"
#include <atomic>
#include <cassert>
#include <climits>
#include <functional>
#include <list>
#include <mutex>
#include <optional>
#include <queue>
#include <thread>

namespace {
using uni_course_cpp::Graph;
using uni_course_cpp::GraphPath;
using uni_course_cpp::INVALID_ID;
using uni_course_cpp::Vertex;
using uni_course_cpp::VertexId;

constexpr int EDGE_STANDART_DISTANCE = 1;
const int MAX_THREADS_COUNT = std::thread::hardware_concurrency();

GraphPath find_shortest_path(const Graph& graph,
                             const VertexId& from_vertex_id,
                             const VertexId& to_vertex_id) {
  assert(graph.vertex_exist(from_vertex_id) && "Vertex doesn't exist");
  assert(graph.vertex_exist(to_vertex_id) && "Vertex doesn't exist");

  const auto vertices_number = graph.get_vertices().size();

  std::vector<GraphPath::Distance> distance(vertices_number, INT_MAX);
  distance[from_vertex_id] = 0;
  std::vector<VertexId> paths_to_verticies(vertices_number, INVALID_ID);
  std::queue<Vertex> vertex_traversal_queue;
  vertex_traversal_queue.push(graph.get_vertex(from_vertex_id));

  while (!vertex_traversal_queue.empty()) {
    const auto current_vertex = vertex_traversal_queue.front();
    vertex_traversal_queue.pop();

    for (const auto& edge_id : current_vertex.get_edge_ids()) {
      const auto connected_vertex_id = [&edge_id, &graph = graph,
                                        &current_vertex]() {
        const auto connected_vertex = graph.get_edge(edge_id).vertex_ids;
        if (current_vertex.id != connected_vertex.first)
          return connected_vertex.first;
        else
          return connected_vertex.second;
      }();

      if (distance[current_vertex.id] + EDGE_STANDART_DISTANCE <
          distance[connected_vertex_id]) {
        vertex_traversal_queue.push(graph.get_vertex(connected_vertex_id));
        distance[connected_vertex_id] =
            distance[current_vertex.id] + EDGE_STANDART_DISTANCE;
        paths_to_verticies[connected_vertex_id] = current_vertex.id;
      }
    }
  }
  std::vector<VertexId> result_path;
  VertexId current_vertex_id = to_vertex_id;
  while (true) {
    result_path.emplace(result_path.begin(), current_vertex_id);
    current_vertex_id = paths_to_verticies[current_vertex_id];
    if (current_vertex_id == INVALID_ID) {
      break;
    }
  };
  return GraphPath(distance[to_vertex_id], std::move(result_path));
}

}  // namespace

namespace uni_course_cpp {

std::vector<GraphPath> GraphTraverser::find_all_paths() const {
  using JobCallback = std::function<void()>;
  auto jobs = std::list<JobCallback>();

  std::vector<GraphPath> paths;
  const auto& vertex_ids_at_last_depth =
      graph_.get_vertex_ids_at_depth(graph_.get_depth() - 1);
  paths.reserve(vertex_ids_at_last_depth.size());

  std::atomic<int> jobs_counter = 0;
  std::mutex mutex;
  for (const auto& vertex_id : vertex_ids_at_last_depth) {
    jobs.emplace_back(
        [this, &graph_ = graph_, &mutex, &jobs_counter, &paths, &vertex_id]() {
          auto path = find_shortest_path(graph_, 0, vertex_id);
          {
            std::lock_guard lock(mutex);
            paths.emplace_back(std::move(path));
          }
          jobs_counter++;
        });
  }

  std::mutex jobs_mutex;
  std::atomic<bool> should_terminate = false;
  const auto worker = [&should_terminate, &jobs_mutex, &jobs]() {
    while (true) {
      if (should_terminate) {
        return;
      }
      const auto job_optional = [&jobs_mutex,
                                 &jobs]() -> std::optional<JobCallback> {
        const std::lock_guard lock(jobs_mutex);
        if (!jobs.empty()) {
          const auto job = jobs.front();
          jobs.pop_front();
          return job;
        }
        return std::nullopt;
      }();
      if (job_optional.has_value()) {
        const auto& job = job_optional.value();
        job();
      }
    }
  };

  const auto threads_number = std::min(
      MAX_THREADS_COUNT, static_cast<int>(vertex_ids_at_last_depth.size()));
  auto threads = std::vector<std::thread>();
  for (int i = 0; i < threads_number; ++i) {
    threads.push_back(std::thread(worker));
  }

  while (jobs_counter < vertex_ids_at_last_depth.size()) {
  }

  should_terminate = true;
  for (auto& thread : threads) {
    thread.join();
  }
  return paths;
}
}  // namespace uni_course_cpp
