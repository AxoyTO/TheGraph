#include "graph_traverser.hpp"
#include <atomic>
#include <functional>
#include <limits>
#include <list>
#include <mutex>
#include <optional>
#include <queue>
#include <random>
#include <thread>
#include <vector>
#include "graph.hpp"

namespace uni_cource_cpp {

using std::lock_guard;
using std::mutex;
using std::thread;
using std::vector;
using VertexId = uni_cource_cpp::Graph::VertexId;
using uni_cource_cpp::GraphPath;
const GraphPath::Distance MAX_DISTANCE = std::numeric_limits<int>::max();
const int kMaxWorkersCount = std::thread::hardware_concurrency();

GraphPath GraphTraverser::find_shortest_path(
    const VertexId& source_vertex_id,
    const VertexId& destination_vertex_id) const {
  const int vertices_count = graph_.get_vertices().size();

  vector<VertexId> visited_vertex_ids(vertices_count, false);

  vector<GraphPath::Distance> distancies_to_vertices(vertices_count,
                                                     MAX_DISTANCE);
  distancies_to_vertices[source_vertex_id] = 0;

  vector<vector<VertexId>> pathes_to_all_vertices(vertices_count);

  std::queue<VertexId> current_vertex_ids;
  current_vertex_ids.push(source_vertex_id);

  while (!current_vertex_ids.empty()) {
    const auto from_vertex_id = current_vertex_ids.front();
    current_vertex_ids.pop();
    visited_vertex_ids[from_vertex_id] = true;

    for (const auto& edge_id :
         graph_.get_vertices().at(from_vertex_id).get_edge_ids()) {
      const auto to_vertex_id = graph_.get_edge(edge_id).to_vertex_id;
      if (!visited_vertex_ids[to_vertex_id]) {
        current_vertex_ids.push(to_vertex_id);
      }
      if (distancies_to_vertices[from_vertex_id] + 1 <
          distancies_to_vertices[to_vertex_id]) {
        distancies_to_vertices[to_vertex_id] =
            distancies_to_vertices[from_vertex_id] + 1;
        pathes_to_all_vertices[to_vertex_id] =
            pathes_to_all_vertices[from_vertex_id];
        pathes_to_all_vertices[to_vertex_id].push_back(from_vertex_id);
      }
      if (to_vertex_id == destination_vertex_id) {
        pathes_to_all_vertices[to_vertex_id].push_back(to_vertex_id);
        return GraphPath(pathes_to_all_vertices[to_vertex_id]);
      }
    }
  }

  throw std::runtime_error("Unforeseen situation");
}

vector<GraphPath> GraphTraverser::find_all_paths() const {
  using JobCallback = std::function<void()>;
  auto jobs = std::list<JobCallback>();
  vector<GraphPath> paths;
  mutex paths_mutex;
  std::atomic<int> jobs_done = 0;
  const auto vertex_ids_on_last_depth =
      graph_.get_vertex_ids_on_depth(graph_.depth());

  for (const auto& to_vertex_id : vertex_ids_on_last_depth) {
    jobs.push_back([this, &to_vertex_id, &jobs_done, &paths_mutex, &paths]() {
      const auto new_path = find_shortest_path(0, to_vertex_id);
      {
        const lock_guard lock(paths_mutex);
        paths.push_back(new_path);
      }
      jobs_done++;
    });
  }
  std::atomic<bool> should_terminate = false;
  mutex jobs_mutex;
  const auto worker = [&should_terminate, &jobs_mutex, &jobs]() {
    while (true) {
      if (should_terminate) {
        return;
      }
      const auto job_optional = [&jobs_mutex,
                                 &jobs]() -> std::optional<JobCallback> {
        const lock_guard lock(jobs_mutex);
        if (!jobs.empty()) {
          const auto next_job = jobs.front();
          jobs.pop_front();
          return next_job;
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
      std::min<int>(kMaxWorkersCount, vertex_ids_on_last_depth.size());
  auto threads = vector<thread>();
  threads.reserve(threads_count);

  for (int i = 0; i < threads_count; i++) {
    threads.push_back(thread(worker));
  }

  while (jobs_done != vertex_ids_on_last_depth.size()) {
  }

  should_terminate = true;
  for (auto& thread : threads) {
    thread.join();
  }
  return paths;
};

}  // namespace uni_cource_cpp
