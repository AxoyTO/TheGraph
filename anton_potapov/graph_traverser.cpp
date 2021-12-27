#include <algorithm>
#include <atomic>
#include <cassert>
#include <map>
#include <mutex>
#include <queue>
#include <set>
#include <thread>
#include <tuple>
#include <utility>
#include <vector>

#include "config.hpp"
#include "graph.hpp"
#include "graph_traverser.hpp"

namespace uni_cource_cpp {
GraphTraverser::GraphTraverser(const Graph& graph) : graph_(graph) {}

std::map<VertexId, int> GraphTraverser::get_updated_depths(
    const Graph& graph,
    const std::map<int, std::set<VertexId>>& known_depths,
    int start_depth) {
  std::map<VertexId, int> depths;
  std::queue<VertexId> bfs_queue;
  std::set<VertexId> used;
  if (start_depth == INIT_DEPTH) {
    const VertexId& first_vertex_id = graph.get_root_vertex_id();
    depths.emplace(first_vertex_id, 0);
    bfs_queue.push(first_vertex_id);
    used.insert(first_vertex_id);
  } else {
    int max_correct_depth = start_depth - 1;
    const auto& vertices_at_max_correct_depth =
        known_depths.at(max_correct_depth);
    for (const auto& vertex_id : vertices_at_max_correct_depth) {
      depths.emplace(vertex_id, max_correct_depth);
      bfs_queue.push(vertex_id);
      used.insert(vertex_id);
    }
    if (max_correct_depth > 0) {
      for (const auto& vertex_id : known_depths.at(max_correct_depth - 1)) {
        used.insert(vertex_id);
      }
    }
  }

  while (!bfs_queue.empty()) {
    const VertexId& current_vertex_id = bfs_queue.front();
    bfs_queue.pop();
    for (const auto& connected_edge_id :
         graph.connected_edges(current_vertex_id)) {
      const Edge& connected_edge = graph.get_edge(connected_edge_id);
      const VertexId& vertex1_id = connected_edge.vertex1_id;
      const VertexId& vertex2_id = connected_edge.vertex2_id;
      const VertexId& connected_vertex_id =
          (current_vertex_id == vertex1_id ? vertex2_id : vertex2_id);
      if (used.find(connected_vertex_id) == used.end()) {
        used.insert(connected_vertex_id);
        depths[connected_vertex_id] = depths[current_vertex_id] + 1;
        bfs_queue.push(connected_vertex_id);
      }
    }
  }
  return depths;
}

GraphPath GraphTraverser::find_shortest_path(
    VertexId source_vertex_id,
    VertexId destination_vertex_id) const {
  std::map<VertexId, GraphPath::Distance> distance;
  distance[source_vertex_id] = 0;
  std::map<VertexId, std::pair<VertexId, EdgeId>> previous_vertex_id;
  std::set<std::pair<GraphPath::Distance, VertexId>> lowest_distance_set;
  lowest_distance_set.insert(std::make_pair(source_vertex_id, 0));
  while (!lowest_distance_set.empty()) {
    const auto current_vertex_id = lowest_distance_set.begin()->second;
    if (current_vertex_id == destination_vertex_id) {
      break;
    }
    const auto current_distance = lowest_distance_set.begin()->first;
    lowest_distance_set.erase(lowest_distance_set.begin());
    for (const auto& edge_id : graph_.connected_edges(current_vertex_id)) {
      const auto& current_edge = graph_.get_edge(edge_id);
      const auto& next_vertex_id = (current_edge.vertex1_id == current_vertex_id
                                        ? current_edge.vertex2_id
                                        : current_edge.vertex1_id);
      if (distance.find(next_vertex_id) == distance.end() ||
          current_distance + 1 < distance[next_vertex_id]) {
        lowest_distance_set.erase(
            std::make_pair(distance[next_vertex_id], next_vertex_id));
        distance[next_vertex_id] = current_distance + 1;
        lowest_distance_set.insert(
            std::make_pair(distance[next_vertex_id], next_vertex_id));
        previous_vertex_id[next_vertex_id] =
            std::make_pair(current_vertex_id, edge_id);
      }
    }
  }
  std::vector<VertexId> vertices_id_path;
  std::vector<EdgeId> edges_id_path;
  auto current_vertex_id = destination_vertex_id;
  while (current_vertex_id != source_vertex_id) {
    assert(previous_vertex_id.find(current_vertex_id) !=
               previous_vertex_id.end() &&
           "dijkstra failed to find the path");
    const auto current_previous_vertex_id =
        previous_vertex_id[current_vertex_id].first;
    const auto current_path_edge = previous_vertex_id[current_vertex_id].second;
    vertices_id_path.push_back(current_vertex_id);
    edges_id_path.push_back(current_path_edge);
    current_vertex_id = current_previous_vertex_id;
  }
  vertices_id_path.push_back(source_vertex_id);
  std::reverse(vertices_id_path.begin(), vertices_id_path.end());
  std::reverse(edges_id_path.begin(), edges_id_path.end());
  return GraphPath(vertices_id_path, edges_id_path);
}

std::vector<GraphPath> GraphTraverser::find_all_paths(
    VertexId source_vertex_id,
    std::set<VertexId> destination_vertices_ids) const {
  std::vector<GraphPath> result;
  std::mutex result_mutex;

  using JobCallback = std::function<void()>;
  auto jobs = std::queue<JobCallback>();

  for (const auto& destination_vertex_id : destination_vertices_ids) {
    jobs.push([this, &result, &result_mutex, &source_vertex_id,
               &destination_vertex_id]() {
      const auto current_path =
          find_shortest_path(source_vertex_id, destination_vertex_id);
      {
        const std::lock_guard result_lock(result_mutex);
        result.push_back(std::move(current_path));
      }
    });
  }

  std::atomic<bool> should_terminate = false;
  std::mutex queue_mutex;
  auto worker = [&should_terminate, &queue_mutex, &jobs]() {
    while (true) {
      if (should_terminate) {
        return;
      }
      const auto job_optional = [&queue_mutex,
                                 &jobs]() -> std::optional<JobCallback> {
        const std::lock_guard queue_lock(queue_mutex);
        if (!jobs.empty()) {
          const auto job = jobs.front();
          jobs.pop();
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
      std::min(config::MAX_THREADS_COUNT,
               static_cast<int>(destination_vertices_ids.size()));
  auto threads = std::vector<std::thread>();
  threads.reserve(threads_count);

  for (int i = 0; i < threads_count; ++i) {
    threads.emplace_back(worker);
  }

  while (true) {
    const std::lock_guard queue_lock(queue_mutex);
    if (jobs.empty()) {
      break;
    }
  }

  should_terminate = true;
  for (auto& thread : threads) {
    thread.join();
  }

  return result;
}
}  // namespace uni_cource_cpp
