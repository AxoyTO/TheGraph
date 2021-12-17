#include <algorithm>
#include <atomic>
#include <cassert>
#include <functional>
#include <iterator>
#include <list>
#include <map>
#include <mutex>
#include <thread>

#include "graph_path.hpp"
#include "graph_traverser.hpp"

namespace {
const uni_cpp_practice::GraphPath::Distance DISTANCE_BETWEEN_NEIGHBOURS = 1;
const int MAX_WORKERS_COUNT = std::thread::hardware_concurrency();
}  // namespace

namespace uni_cpp_practice {

VertexId GraphTraverser::next_vertex_to_check(
    std::map<VertexId, VertexInfo>& vertices_info) const {
  const auto& vertices = graph_.get_vertices();

  GraphPath::Distance min_distance = INT32_MAX;
  VertexId vertex_with_min_dist = vertices[0].id;

  for (const auto& vertex : vertices) {
    if (!vertices_info[vertex.id].visited &&
        vertices_info[vertex.id].distance < min_distance) {
      min_distance = vertices_info[vertex.id].distance;
      vertex_with_min_dist = vertex.id;
    }
  }

  return vertex_with_min_dist;
}

GraphPath GraphTraverser::find_shortest_path(
    const VertexId& source_verex_id,
    const VertexId& destination_vertex_id) const {
  assert(graph_.check_vertex_existence(source_verex_id) &&
         "Attemptig to access to nonexistent vertex: Error.");
  assert(graph_.check_vertex_existence(destination_vertex_id) &&
         "Attemptig to access to nonexistent vertex: Error.");

  std::map<VertexId, VertexInfo> vertices_info;

  const auto& vertices = graph_.get_vertices();

  for (const auto& vertex_id : vertices) {
    vertices_info[vertex_id.id].distance = INT32_MAX;
    vertices_info[vertex_id.id].visited = false;
  }

  vertices_info[source_verex_id].distance = 0;
  VertexId current_vertex_id = source_verex_id;

  while (current_vertex_id != destination_vertex_id) {
    const auto& current_vertex = graph_.get_vertex(current_vertex_id);
    const auto neighbours = graph_.get_vertex_neighbours(current_vertex_id);

    for (const auto& neighbour : neighbours) {
      if (!vertices_info[neighbour].visited &&
          vertices_info[neighbour].distance >
              vertices_info[current_vertex_id].distance +
                  DISTANCE_BETWEEN_NEIGHBOURS) {
        vertices_info[neighbour].distance =
            vertices_info[current_vertex_id].distance +
            DISTANCE_BETWEEN_NEIGHBOURS;
        vertices_info[neighbour].nearest_neighbour = current_vertex_id;
      }
    }

    vertices_info[current_vertex_id].visited = true;

    current_vertex_id = next_vertex_to_check(vertices_info);
  }

  std::vector<VertexId> path;
  current_vertex_id = destination_vertex_id;
  while (current_vertex_id != source_verex_id) {
    path.push_back(current_vertex_id);
    current_vertex_id = vertices_info[current_vertex_id].nearest_neighbour;
  }
  path.push_back(source_verex_id);

  std::reverse(path.begin(), path.end());

  GraphPath result(path);

  return result;
}

std::vector<GraphPath> GraphTraverser::find_all_paths() const {
  using JobCallback = std::function<void()>;
  auto jobs = std::list<JobCallback>();
  std::mutex mutex;

  std::vector<GraphPath> paths;

  VertexId root_vertex = graph_.depths_map_[0][0];
  const auto& last_depth_vertices =
      graph_.depths_map_[graph_.depths_map_.size() - 1];

  std::atomic<int> done_jobs_number = 0;

  for (int i = 0; i < last_depth_vertices.size(); i++) {
    jobs.push_back([&root_vertex, &last_depth_vertices, &done_jobs_number,
                    &mutex, i, &paths, this]() {
      GraphPath path = find_shortest_path(root_vertex, last_depth_vertices[i]);
      {
        const std::lock_guard lock(mutex);
        paths.push_back(path);
      }
      done_jobs_number++;
    });
  }

  std::mutex jobs_mutex;
  std::atomic<bool> should_terminate = false;
  const auto worker = [&jobs, &jobs_mutex, &should_terminate]() {
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
      std::min(MAX_WORKERS_COUNT, (int)last_depth_vertices.size());
  auto threads = std::vector<std::thread>();
  threads.reserve(threads_count);

  for (int i = 0; i < threads_count; i++) {
    threads.push_back(std::thread(worker));
  }

  while (done_jobs_number < last_depth_vertices.size()) {
  }

  should_terminate = true;
  for (auto& thread : threads) {
    thread.join();
  }

  return paths;
}

}  // namespace uni_cpp_practice
