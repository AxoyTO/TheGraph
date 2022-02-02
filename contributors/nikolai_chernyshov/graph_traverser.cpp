#include "graph_traverser.hpp"
#include <algorithm>
#include <atomic>
#include <cassert>
#include <climits>
#include <list>
#include <mutex>
#include <optional>
#include <queue>
#include <thread>

namespace {
const int MAX_WORKERS_COUNT = std::thread::hardware_concurrency();
constexpr int MAX_DISTANCE = INT_MAX;
constexpr int UNDEFINED_ID = -1;
}  // namespace

namespace uni_course_cpp {

GraphPath GraphTraverser::find_shortest_path(
    const VertexId& source_vertex_id,
    const VertexId& destination_vertex_id) const {
  assert(graph_.has_vertex(source_vertex_id));
  assert(graph_.has_vertex(destination_vertex_id));

  const int vertices_count = graph_.get_vertices().size();

  std::vector<GraphPath::Distance> distances(vertices_count, MAX_DISTANCE);
  distances[source_vertex_id] = 0;

  std::queue<VertexId> queue;
  queue.push(source_vertex_id);

  std::vector<VertexId> previous_ids(vertices_count, UNDEFINED_ID);

  while (!queue.empty()) {
    const auto current_vertex_id = queue.front();
    queue.pop();

    for (const auto& vertex_id :
         graph_.get_linked_vertex_ids(current_vertex_id)) {
      if (distances[current_vertex_id] + 1 < distances[vertex_id]) {
        queue.push(vertex_id);
        distances[vertex_id] = distances[current_vertex_id] + 1;
        previous_ids[vertex_id] = current_vertex_id;
        if (destination_vertex_id == vertex_id) {
          break;
        }
      }
    }
  }

  const auto path = [&destination_vertex_id, &source_vertex_id,
                     &previous_ids]() {
    std::vector<VertexId> result;
    VertexId vertex_id = destination_vertex_id;
    while (vertex_id != source_vertex_id) {
      result.push_back(vertex_id);
      vertex_id = previous_ids[vertex_id];
    }
    result.push_back(source_vertex_id);
    std::reverse(result.begin(), result.end());
    return result;
  }();

  return GraphPath(path);
}

std::vector<GraphPath> GraphTraverser::find_all_paths() const {
  using JobCallback = std::function<void()>;
  auto jobs = std::list<JobCallback>();

  std::mutex jobs_mutex;
  std::mutex paths_mutex;
  std::atomic<int> finished_jobs_num = 0;
  std::atomic<bool> should_terminate = false;

  const auto& last_depth_vertex_ids = graph_.get_depth_map().back();

  std::vector<GraphPath> paths;
  paths.reserve(last_depth_vertex_ids.size());

  for (const auto& vertex_id : last_depth_vertex_ids) {
    jobs.push_back(
        [&paths, &paths_mutex, &vertex_id, &finished_jobs_num, this]() {
          GraphPath path = find_shortest_path(0, vertex_id);
          {
            std::lock_guard lock(paths_mutex);
            paths.push_back(path);
          }
          finished_jobs_num++;
        });
  }

  const auto worker = [&should_terminate, &jobs_mutex, &jobs]() {
    while (true) {
      if (should_terminate) {
        return;
      }
      const auto job_optional = [&jobs,
                                 &jobs_mutex]() -> std::optional<JobCallback> {
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

  const auto threads_count =
      std::min(MAX_WORKERS_COUNT, (int)last_depth_vertex_ids.size());
  auto threads = std::vector<std::thread>();
  threads.reserve(threads_count);

  for (int i = 0; i < threads_count; i++) {
    threads.push_back(std::thread(worker));
  }

  while (finished_jobs_num < last_depth_vertex_ids.size()) {
  }

  should_terminate = true;
  for (auto& thread : threads) {
    thread.join();
  }

  return paths;
}

}  // namespace uni_course_cpp
