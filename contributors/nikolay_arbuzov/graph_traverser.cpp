#include "graph_traverser.hpp"
#include <atomic>
#include <climits>
#include <functional>
#include <list>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>
#include "graph.hpp"
#include "graph_path.hpp"

namespace {
const unsigned long MAX_WORKERS_COUNT = std::thread::hardware_concurrency();
}

namespace uni_course_cpp {

GraphPath GraphTraverser::find_shortest_path(
    const Graph& graph,
    const Graph::VertexId& source_vertex_id,
    const Graph::VertexId& destination_vertex_id) const {
  std::unordered_map<Graph::VertexId, GraphPath::Distance> distance_map;

  const int vertices_count = graph.vertices_id_counter();
  const auto& source_vertex = graph.get_vertex(source_vertex_id);

  std::vector<Graph::VertexId> vertices(vertices_count, 0);
  vertices[source_vertex_id] = 1;

  std::vector<GraphPath::Distance> distance(vertices_count, INT_MAX);
  distance[source_vertex_id] = 0;
  std::queue<Graph::Vertex> vertices_queue;
  vertices_queue.push(source_vertex);

  std::vector<std::vector<Graph::VertexId>> all_pathes(vertices_count);
  std::vector<Graph::VertexId> source_vector{source_vertex_id};
  all_pathes[source_vertex_id] = source_vector;

  while (!vertices_queue.empty()) {
    const auto current_vertex = vertices_queue.front();
    vertices_queue.pop();
    for (const auto& edge_id : graph.connected_edge_ids(current_vertex.id)) {
      const auto& edge = graph.get_edge(edge_id);
      Graph::VertexId next_vertex_id;
      if (edge.from_vertex_id != current_vertex.id) {
        next_vertex_id = edge.from_vertex_id;
      } else if (edge.to_vertex_id != current_vertex.id) {
        next_vertex_id = edge.to_vertex_id;
      }
      const auto& next_vertex = graph.get_vertex(next_vertex_id);
      if (distance[current_vertex.id] + 1 < distance[next_vertex_id]) {
        vertices_queue.push(next_vertex);
        distance[next_vertex_id] = distance[current_vertex.id] + 1;
        all_pathes[next_vertex_id] = all_pathes[current_vertex.id];
        all_pathes[next_vertex_id].push_back(next_vertex_id);
        if (destination_vertex_id == next_vertex_id) {
          GraphPath r_path(all_pathes[next_vertex_id],
                           distance[next_vertex_id]);
          return r_path;
        }
      }
    }
  }
  throw std::runtime_error("Vertices are not connected");
}
std::vector<GraphPath> GraphTraverser::traverse_graph() {
  std::list<std::function<void()>> jobs;
  std::atomic<int> jobs_done = 0;
  std::mutex path_mutex;
  const auto& vertex_ids = graph_.get_vertex_ids_on_depth(graph_.depth());
  std::vector<GraphPath> pathes;
  pathes.reserve(vertex_ids.size());

  for (const auto& vertex_id : vertex_ids)
    jobs.emplace_back([this, &graph_ = graph_, &jobs_done, &vertex_id, &pathes,
                       &path_mutex]() {
      auto path = find_shortest_path(graph_, 0, vertex_id);
      {
        const std::lock_guard lock(path_mutex);
        pathes.emplace_back(std::move(path));
      }
      jobs_done++;
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

  while (jobs_done < vertex_ids.size()) {
  }

  should_terminate = true;
  for (auto& thread : threads) {
    thread.join();
  }

  return pathes;
}
}  // namespace uni_course_cpp
