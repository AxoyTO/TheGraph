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
constexpr uni_cpp_practice::GraphPath::Distance DISTANCE_BETWEEN_NEIGHBOURS = 1;
const int MAX_WORKERS_COUNT = std::thread::hardware_concurrency();

struct VertexInfo {
    uni_cpp_practice::VertexId previous_vertex_in_path;
    uni_cpp_practice::GraphPath::Distance distance;
    bool visited;
  };

uni_cpp_practice::VertexId next_vertex_to_check(
      std::map<uni_cpp_practice::VertexId, VertexInfo>& vertices_info, const uni_cpp_practice::Graph& graph){
  const auto& vertices = graph.get_vertices();

  uni_cpp_practice::GraphPath::Distance min_distance = INT32_MAX;
  uni_cpp_practice::VertexId vertex_with_min_dist = vertices[0].id;

  for (const auto& vertex : vertices) {
    if (!vertices_info[vertex.id].visited &&
        vertices_info[vertex.id].distance < min_distance) {
      min_distance = vertices_info[vertex.id].distance;
      vertex_with_min_dist = vertex.id;
    }
  }

  return vertex_with_min_dist;
}
}  // namespace

namespace uni_cpp_practice {

GraphPath GraphTraverser::find_shortest_path(
    const VertexId& source_verex_id,
    const VertexId& destination_vertex_id) const {
  assert(graph_.check_vertex_existence(source_verex_id) &&
         "Attemptig to access to nonexistent vertex: Error.");
  assert(graph_.check_vertex_existence(destination_vertex_id) &&
         "Attemptig to access to nonexistent vertex: Error.");

  std::map<VertexId, VertexInfo> vertices_info;

  for (const auto& vertex_id : graph_.get_vertices()) {
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
        vertices_info[neighbour].previous_vertex_in_path = current_vertex_id;
      }
    }

    vertices_info[current_vertex_id].visited = true;

    current_vertex_id = next_vertex_to_check(vertices_info, graph_);
  }

  const auto path = [&current_vertex_id, &destination_vertex_id,
                     &source_verex_id, &vertices_info]() {
    std::vector<VertexId> result;
    current_vertex_id = destination_vertex_id;
    while (current_vertex_id != source_verex_id) {
      result.push_back(current_vertex_id);
      current_vertex_id =
          vertices_info[current_vertex_id].previous_vertex_in_path;
    }
    result.push_back(source_verex_id);

    std::reverse(result.begin(), result.end());

    return result;
  };

  return GraphPath(path());
}

std::vector<GraphPath> GraphTraverser::find_all_paths() const {
  using JobCallback = std::function<void()>;
  auto jobs = std::list<JobCallback>();
  std::mutex paths_mutex;

  std::vector<GraphPath> paths;

  const VertexId root_vertex_id = graph_.depths_map_[0][0];
  const auto& last_depth_vertices =
      graph_.depths_map_[graph_.depths_map_.size() - 1];

  std::atomic<int> done_jobs_number = 0;

  for (int i = 0; i < last_depth_vertices.size(); i++) {
    jobs.push_back([&root_vertex_id, &last_depth_vertices, &done_jobs_number,
                    &paths_mutex, i, &paths, this]() {
      GraphPath path =
          find_shortest_path(root_vertex_id, last_depth_vertices[i]);
      {
        const std::lock_guard lock(paths_mutex);
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
