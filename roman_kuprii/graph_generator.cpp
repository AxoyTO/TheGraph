#include <atomic>
#include <functional>
#include <list>
#include <mutex>
#include <optional>
#include <random>
#include <thread>
#include <vector>

#include "graph.hpp"
#include "graph_generator.hpp"

namespace {

double get_real_random_number() {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_real_distribution<> dis(0, 1);
  return dis(gen);
}

int get_int_random_number(int upper_bound) {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> dis(0, upper_bound);
  return dis(gen);
}

constexpr double GREEN_TRASHOULD = 0.1;
constexpr double BLUE_TRASHOULD = 0.25;
constexpr double RED_TRASHOULD = 0.33;

constexpr int MAX_THREADS_COUNT = 4;

using std::vector;

using uni_cpp_practice::Edge;
using uni_cpp_practice::Graph;
using uni_cpp_practice::INVALID_ID;
using uni_cpp_practice::Vertex;
using uni_cpp_practice::VertexId;

void add_blue_edges(Graph& work_graph, std::mutex& add_edge_mutex) {
  const int graph_depth = work_graph.get_depth();
  for (int current_depth = 1; current_depth <= graph_depth; current_depth++) {
    const auto& vertex_ids_at_current_depth =
        work_graph.get_vertex_ids_at_depth(current_depth);

    std::array<VertexId, 2> adjacent_vertices = {INVALID_ID, INVALID_ID};
    for (const auto& vertex_id : vertex_ids_at_current_depth) {
      if (adjacent_vertices[0] == INVALID_ID) {
        adjacent_vertices[0] = vertex_id;
      } else if (adjacent_vertices[1] == INVALID_ID) {
        adjacent_vertices[1] = vertex_id;
        if (get_real_random_number() < BLUE_TRASHOULD) {
          std::lock_guard lock(add_edge_mutex);
          work_graph.connect_vertices(adjacent_vertices[0],
                                      adjacent_vertices[1], false);
        }
      } else {
        adjacent_vertices[0] = adjacent_vertices[1];
        adjacent_vertices[1] = vertex_id;
        if (get_real_random_number() < BLUE_TRASHOULD) {
          std::lock_guard lock(add_edge_mutex);
          work_graph.connect_vertices(adjacent_vertices[0],
                                      adjacent_vertices[1], false);
        }
      }
    }
  }
}

void add_green_edges(Graph& work_graph, std::mutex& add_edge_mutex) {
  for (const auto& start_vertex : work_graph.get_vertices_map())
    if (get_real_random_number() < GREEN_TRASHOULD) {
      std::lock_guard lock(add_edge_mutex);
      work_graph.connect_vertices(start_vertex.first, start_vertex.first,
                                  false);
    }
}

void add_red_edges(Graph& work_graph, std::mutex& add_edge_mutex) {
  const int graph_depth = work_graph.get_depth();
  for (const auto& start_vertex : work_graph.get_vertices_map()) {
    if (get_real_random_number() < RED_TRASHOULD) {
      if (start_vertex.second.depth + 2 <= graph_depth) {
        const auto& red_vertices_ids =
            work_graph.get_vertex_ids_at_depth(start_vertex.second.depth + 2);
        if (red_vertices_ids.size() > 0) {
          std::lock_guard lock(add_edge_mutex);
          work_graph.connect_vertices(start_vertex.first,
                                      red_vertices_ids[get_int_random_number(
                                          red_vertices_ids.size() - 1)],
                                      false);
        }
      }
    }
  }
}

void add_yellow_edges(Graph& work_graph, std::mutex& add_edge_mutex) {
  const int graph_depth = work_graph.get_depth();
  for (const auto& start_vertex : work_graph.get_vertices_map()) {
    const double probability = static_cast<double>(start_vertex.second.depth) /
                               static_cast<double>(graph_depth);
    if (get_real_random_number() < probability) {
      vector<VertexId> yellow_vertices_ids;
      if (start_vertex.second.depth + 1 <= graph_depth) {
        const auto& vertex_on_next_depth =
            work_graph.get_vertex_ids_at_depth(start_vertex.second.depth + 1);
        for (const auto& vertex_id : vertex_on_next_depth) {
          const auto is_connected = [&work_graph, &add_edge_mutex,
                                     &start_vertex, &vertex_id]() {
            const std::lock_guard lock(add_edge_mutex);
            return work_graph.is_connected(start_vertex.first, vertex_id);
          }();
          if (!is_connected)
            yellow_vertices_ids.push_back(vertex_id);
        }
        if (yellow_vertices_ids.size() > 0) {
          std::lock_guard lock(add_edge_mutex);
          work_graph.connect_vertices(start_vertex.first,
                                      yellow_vertices_ids[get_int_random_number(
                                          yellow_vertices_ids.size() - 1)],
                                      false);
        }
      }
    }
  }
}

void paint_edges(Graph& work_graph) {
  std::mutex add_edges_mutex;
  std::thread blue_thread([&work_graph, &add_edges_mutex]() {
    add_blue_edges(work_graph, add_edges_mutex);
  });
  std::thread green_thread([&work_graph, &add_edges_mutex]() {
    add_green_edges(work_graph, add_edges_mutex);
  });
  std::thread red_thread([&work_graph, &add_edges_mutex]() {
    add_red_edges(work_graph, add_edges_mutex);
  });
  std::thread yellow_thread([&work_graph, &add_edges_mutex]() {
    add_yellow_edges(work_graph, add_edges_mutex);
  });
  blue_thread.join();
  green_thread.join();
  red_thread.join();
  yellow_thread.join();
}

}  // namespace

namespace uni_cpp_practice {

void GraphGenerator::generate_gray_branch(Graph& work_graph,
                                          std::mutex& graph_mutex,
                                          const VertexId& parent_vertex_id,
                                          int current_depth) const {
  const int depth = params_.depth;
  const VertexId new_vertex_id = [&work_graph, &graph_mutex,
                                  &parent_vertex_id]() {
    const std::lock_guard lock(graph_mutex);
    const auto new_vertex_id = work_graph.add_vertex();
    work_graph.connect_vertices(parent_vertex_id, new_vertex_id, true);
    return new_vertex_id;
  }();

  if (current_depth == depth)
    return;

  const double probability =
      static_cast<double>(current_depth) / static_cast<double>(depth);

  for (int i = 0; i < params_.new_vertices_num; i++) {
    if (get_real_random_number() > probability) {
      generate_gray_branch(work_graph, graph_mutex, new_vertex_id,
                           current_depth + 1);
    }
  }
}

void GraphGenerator::generate_new_vertices(
    Graph& graph,
    const VertexId& parent_vertex_id) const {
  std::list<std::function<void()>> jobs;
  std::atomic<int> completed_jobs = 0;
  std::mutex graph_mutex;
  for (int i = 0; i < params_.new_vertices_num; i++)
    jobs.emplace_back(
        [this, &graph, &completed_jobs, &graph_mutex, parent_vertex_id]() {
          generate_gray_branch(graph, graph_mutex, parent_vertex_id, 1);
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

  const auto threads_number =
      std::min(params_.new_vertices_num, MAX_THREADS_COUNT);
  auto threads = std::vector<std::thread>();
  threads.reserve(threads_number);

  for (int i = 0; i < threads_number; ++i) {
    threads.emplace_back(worker);
  }

  while (completed_jobs != params_.new_vertices_num) {
  }

  should_terminate = true;
  for (auto& thread : threads) {
    thread.join();
  }
}

Graph GraphGenerator::generate() const {
  auto graph = Graph();
  const auto parent_vertex_id = graph.add_vertex();
  generate_new_vertices(graph, parent_vertex_id);
  paint_edges(graph);
  return graph;
}

}  // namespace uni_cpp_practice
