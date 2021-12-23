#include "graph_traverser.hpp"
#include "graph.hpp"

#include <atomic>
#include <cassert>
#include <functional>
#include <limits>
#include <list>
#include <mutex>
#include <optional>
#include <queue>
#include <thread>
#include <vector>

namespace {
constexpr int MAX_DISTANCE = std::numeric_limits<int>::max();
const int MAX_WORKERS_COUNT = std::thread::hardware_concurrency();
}  // namespace

namespace uni_cource_cpp {
GraphPath GraphTraverser::find_shortest_path(
    const Graph& graph,
    const VertexId& source_vertex_id,
    const VertexId& destination_vertex_id) const {
  assert(graph_.has_vertex_id(source_vertex_id) && "Vertex doesn't exists");
  assert(graph_.has_vertex_id(destination_vertex_id) &&
         "Vertex doesn't exists");

  const int vertices_number = graph.get_vertices().size();
  const auto& source_vertex = graph.get_vertices().at(source_vertex_id);

  std::vector<GraphPath::Distance> distance(vertices_number, MAX_DISTANCE);
  distance[source_vertex_id] = 0;

  std::queue<Graph::Vertex> vertices_queue;
  vertices_queue.push(source_vertex);

  std::vector<std::vector<VertexId>> all_pathes(vertices_number);
  std::vector<VertexId> source_vector(1, source_vertex_id);
  all_pathes[source_vertex_id] = source_vector;

  while (!vertices_queue.empty()) {
    const auto current_vertex = vertices_queue.front();
    vertices_queue.pop();

    for (const auto& edge_id : current_vertex.get_edge_ids()) {
      const auto& edge = graph.get_edges().at(edge_id);
      VertexId next_vertex_id = edge.vertex_end;
      const auto& next_vertex = graph.get_vertices().at(next_vertex_id);

      if (distance[current_vertex.id] + 1 < distance[next_vertex_id]) {
        vertices_queue.push(next_vertex);
        distance[next_vertex_id] = distance[current_vertex.id] + 1;

        all_pathes[next_vertex_id] = all_pathes[current_vertex.id];
        all_pathes[next_vertex_id].push_back(next_vertex_id);

        if (destination_vertex_id == next_vertex_id) {
          GraphPath r_path(all_pathes[next_vertex_id]);
          return r_path;
        }
      }
    }
  }

  throw std::logic_error("Vertices dont connected");
}

std::vector<GraphPath> GraphTraverser::find_all_paths() {
  std::list<std::function<void()>> jobs;
  std::atomic<int> finished_jobs_num = 0;
  std::mutex path_mutex;
  const auto& vertex_ids = graph_.get_vertex_ids_at(graph_.get_depth());
  std::vector<GraphPath> pathes;
  pathes.reserve(vertex_ids.size());

  for (const auto& vertex_id : vertex_ids)
    jobs.emplace_back([this, &graph_ = graph_, &finished_jobs_num, &vertex_id,
                       &pathes, &path_mutex]() {
      auto path = find_shortest_path(graph_, 0, vertex_id);
      {
        std::lock_guard lock(path_mutex);
        pathes.emplace_back(path);
      }
      finished_jobs_num++;
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
        if (jobs.empty())
          return std::nullopt;

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

  const auto threads_num =
      std::min(static_cast<int>(vertex_ids.size()), MAX_WORKERS_COUNT);
  auto threads = std::vector<std::thread>();
  threads.reserve(threads_num);

  for (int i = 0; i < threads_num; ++i) {
    threads.emplace_back(worker);
  }

  while (finished_jobs_num < vertex_ids.size()) {
  }

  should_terminate = true;
  for (auto& thread : threads) {
    thread.join();
  }

  return pathes;
}

}  // namespace uni_cource_cpp
