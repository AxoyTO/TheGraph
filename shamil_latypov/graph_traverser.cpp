#include "graph_traverser.hpp"
#include <algorithm>
#include <atomic>
#include <functional>
#include <list>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>
#include "graph.hpp"
#include "graph_path.hpp"

namespace {

using uni_cource_cpp::Graph;
using uni_cource_cpp::VertexId;

int get_current_vertex_num(const VertexId& vertex_id, const Graph& graph) {
  int num = 0;
  for (auto vertex_it = graph.get_vertices().begin();
       vertex_it < graph.get_vertices().end(); vertex_it++) {
    if (vertex_it->get_id() == vertex_id) {
      num = static_cast<int>(vertex_it - (graph.get_vertices()).begin());
      break;
    }
  }
  return num;
}

}  // namespace

namespace uni_cource_cpp {

void print_path(GraphPath a) {
  for (auto& i : a.vertex_ids) {
    std::cout << "VV " << i << " VV ";
  }
  std::cout << "\n";
}

const int MAX_WORKERS_COUNT = std::thread::hardware_concurrency();
constexpr bool VERTEX_CHECKED = 1;
constexpr int MAX_DISTANCE = 0x7fffffff;
constexpr VertexId FIRST_VERTEX = 0;

GraphPath GraphTraverser::find_shortest_path(
    VertexId source_vertex_id,
    VertexId destination_vertex_id) const {
  int vertices_number = graph_.get_vertices_count();
  const auto& source_vertex = graph_.get_vertex(source_vertex_id);

  std::vector<GraphPath::Distance> distance(vertices_number, MAX_DISTANCE);
  distance[source_vertex_id] = 0;

  std::queue<Vertex> vertices_queue;
  vertices_queue.push(source_vertex);

  std::vector<std::vector<VertexId>> all_pathes(vertices_number);
  std::vector<std::vector<EdgeId>> all_pathes_edge_ids(vertices_number);
  std::vector<VertexId> begin_vector(1, source_vertex_id);

  int current_vertex_num = get_current_vertex_num(source_vertex_id, graph_);
  all_pathes[current_vertex_num] = begin_vector;

  while (!vertices_queue.empty()) {
    const auto current_vertex = vertices_queue.front();
    vertices_queue.pop();

    for (const auto& edge_id : current_vertex.get_edge_ids()) {
      VertexId next_vertex_id = (graph_.get_edge(edge_id)).get_vertex2_id();
      const auto& next_vertex = graph_.get_vertex(next_vertex_id);

      current_vertex_num =
          get_current_vertex_num(current_vertex.get_id(), graph_);
      int next_vertex_num = get_current_vertex_num(next_vertex_id, graph_);

      if (distance[current_vertex_num] + 1 < distance[next_vertex_num]) {
        vertices_queue.push(next_vertex);
        distance[next_vertex_num] = distance[current_vertex_num] + 1;

        all_pathes[next_vertex_num] = all_pathes[current_vertex_num];
        all_pathes[next_vertex_num].push_back(next_vertex_id);

        all_pathes_edge_ids[next_vertex_num] =
            all_pathes_edge_ids[current_vertex_num];
        all_pathes_edge_ids[next_vertex_num].push_back(edge_id);

        if (destination_vertex_id == next_vertex_id) {
          auto new_path = GraphPath(all_pathes[next_vertex_num],
                                    all_pathes_edge_ids[next_vertex_num]);
          return new_path;
        }
      }
    }
  }
  throw std::runtime_error("Failed to find path");
}

std::vector<GraphPath> GraphTraverser::find_all_paths() const {
  using JobCallback = std::function<void()>;
  auto jobs = std::list<JobCallback>();

  std::vector<GraphPath> graph_paths;
  std::atomic<int> paths_found = 0;
  std::mutex new_path_lock;

  for (const auto& vert_ids :
       graph_.get_vertices_in_depth(graph_.get_depth())) {
    jobs.emplace_back(
        [this, &vert_ids, &paths_found, &new_path_lock, &graph_paths]() {
          GraphPath new_path = find_shortest_path(FIRST_VERTEX, vert_ids);
          {
            std::lock_guard lock(new_path_lock);
            graph_paths.push_back(new_path);
          }
          paths_found++;
        });
  }

  std::atomic<bool> should_terminate = false;
  std::mutex jobs_lock;

  auto worker = [&should_terminate, &jobs_lock, &jobs]() {
    while (true) {
      if (should_terminate) {
        return;
      }
      const auto job_optional = [&jobs_lock,
                                 &jobs]() -> std::optional<JobCallback> {
        std::lock_guard lock(jobs_lock);
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
      std::min(MAX_WORKERS_COUNT,
               static_cast<int>(
                   graph_.get_vertices_in_depth(graph_.get_depth()).size()));
  auto threads = std::vector<std::thread>();
  threads.reserve(threads_count);

  for (int i = 0; i < threads_count; i++) {
    threads.emplace_back(worker);
  }

  while (paths_found !=
         graph_.get_vertices_in_depth(graph_.get_depth()).size()) {
  }

  should_terminate = true;
  for (auto& thread : threads) {
    thread.join();
  }
  return graph_paths;
}

}  // namespace uni_cource_cpp