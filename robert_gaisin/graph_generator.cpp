#include "graph_generator.hpp"
#include "graph.hpp"

#include <atomic>
#include <functional>
#include <iostream>
#include <list>
#include <mutex>
#include <random>
#include <thread>

namespace {

using uni_cource_cpp::EdgeColor;
using uni_cource_cpp::Graph;
using uni_cource_cpp::Params;
using uni_cource_cpp::VertexId;

const int MAX_THREADS_COUNT = std::thread::hardware_concurrency();

constexpr int PROBA_GRAY_BEGIN = 100;
constexpr int PROBA_RED = 33;
constexpr int PROBA_GREEN = 10;
constexpr int PROBA_BLUE = 25;

bool to_be_or_not_to_be(int proba) {
  std::random_device rd;
  std::mt19937 mersenne(rd());
  return (int)(mersenne() % 100 + 1) <= proba;
}

void generate_green_edges(Graph& graph, std::mutex& mutex) {
  for (auto& vertex : graph.vertices()) {
    if (to_be_or_not_to_be(PROBA_GREEN)) {
      const std::lock_guard lock(mutex);
      graph.add_edge(vertex.id, vertex.id, EdgeColor::Green);
    }
  }
}

void generate_blue_edges(Graph& graph, std::mutex& mutex) {
  for (const auto& vector_ids : graph.depth_map()) {
    for (auto vertex_id_iter = vector_ids.begin();
         vertex_id_iter < vector_ids.end() - 1; ++vertex_id_iter) {
      if (to_be_or_not_to_be(PROBA_BLUE)) {
        const std::lock_guard lock(mutex);
        graph.add_edge(*vertex_id_iter, *(vertex_id_iter + 1), EdgeColor::Blue);
      }
    }
  }
}

std::vector<VertexId> get_unconnected_vertex_ids(
    const std::vector<VertexId>& layer,
    const VertexId& vert_id,
    const Graph& graph) {
  std::vector<VertexId> returned_vector;
  for (auto& vertex_id : layer) {
    if (graph.is_connected(vert_id, vertex_id))
      continue;
    returned_vector.push_back(vertex_id);
  }
  return returned_vector;
}

VertexId get_random_vertex_id(std::vector<VertexId> set_of_vertices_id) {
  std::random_device rd;
  std::mt19937 mersenne(rd());
  if (set_of_vertices_id.size())
    return set_of_vertices_id[mersenne() % set_of_vertices_id.size()];
  return 0;
}

void generate_yellow_edges(Graph& graph, std::mutex& mutex) {
  if (graph.depth() < 2)
    return;

  for (auto vertex_ids_at_depth = graph.depth_map().begin();
       vertex_ids_at_depth != graph.depth_map().end() - 1;
       ++vertex_ids_at_depth) {
    const int depth = graph.get_vertex((*vertex_ids_at_depth).front()).depth;
    const int proba_yellow = ((float)depth / (float)(graph.depth() - 1)) * 100;
    for (auto vertex_id = (*vertex_ids_at_depth).begin();
         vertex_id != (*vertex_ids_at_depth).end(); ++vertex_id) {
      if (to_be_or_not_to_be(proba_yellow)) {
        const std::lock_guard lock(mutex);
        std::vector<VertexId> vertices_to_connect = get_unconnected_vertex_ids(
            *(vertex_ids_at_depth + 1), *vertex_id, graph);
        const int vertex_to_attach = get_random_vertex_id(vertices_to_connect);
        if (vertex_to_attach) {
          graph.add_edge(*vertex_id, vertex_to_attach, EdgeColor::Yellow);
        }
      }
    }
  }
}

void generate_red_edges(Graph& graph, std::mutex& mutex) {
  if (graph.depth() < 3)
    return;

  for (auto vertex_ids_at_depth = graph.depth_map().begin();
       vertex_ids_at_depth != graph.depth_map().end() - 2;
       ++vertex_ids_at_depth) {
    for (auto vertex_id = (*vertex_ids_at_depth).begin();
         vertex_id != (*vertex_ids_at_depth).end(); ++vertex_id) {
      if (to_be_or_not_to_be(PROBA_RED)) {
        const int vertex_to_attach =
            get_random_vertex_id(*(vertex_ids_at_depth + 2));
        const std::lock_guard lock(mutex);
        graph.add_edge(*vertex_id, vertex_to_attach, EdgeColor::Red);
      }
    }
  }
}

}  // namespace
namespace uni_cource_cpp {
void GraphGenerator::generate_gray_branch(Graph& graph,
                                          int depth,
                                          const VertexId& vertex_id,
                                          std::mutex& mutex) const {
  if (depth == params_.depth) {
    return;
  }

  int proba_gray = PROBA_GRAY_BEGIN * (1.0 - (float)depth / params_.depth);
  if (!to_be_or_not_to_be(proba_gray)) {
    return;
  }

  const VertexId new_vertex_id = [&mutex, &graph, &vertex_id]() {
    const std::lock_guard lock(mutex);
    const auto new_vertex_id = graph.add_vertex();
    graph.add_edge(vertex_id, new_vertex_id, EdgeColor::Gray);
    return new_vertex_id;
  }();

  for (int i = 0; i < params_.new_vertices_count; i++) {
    generate_gray_branch(graph, depth + 1, new_vertex_id, mutex);
  }
}

void GraphGenerator::generate_gray_edges(Graph& graph) const {
  using JobCallback = std::function<void()>;
  auto jobs = std::list<JobCallback>();
  std::mutex graph_mutex;

  const VertexId first_vertex_id = graph.add_vertex();
  std::atomic<int> done_jobs_number = 0;
  for (int i = 0; i < params_.new_vertices_count; ++i) {
    jobs.push_back(
        [&graph, &graph_mutex, &done_jobs_number, &first_vertex_id, this]() {
          generate_gray_branch(graph, 0, first_vertex_id, graph_mutex);
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
          const auto job = jobs.back();
          jobs.pop_back();
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
      std::min(MAX_THREADS_COUNT, params_.new_vertices_count);
  auto threads = std::vector<std::thread>();
  threads.reserve(threads_count);

  for (int i = 0; i < threads_count; i++) {
    threads.push_back(std::thread(worker));
  }

  while (done_jobs_number < params_.new_vertices_count) {
  }

  should_terminate = true;
  for (auto& thread : threads) {
    thread.join();
  }
}

Graph GraphGenerator::generate() const {
  auto graph = Graph();

  generate_gray_edges(graph);

  std::mutex mutex;

  std::thread green_thread(
      [&graph, &mutex]() { generate_green_edges(graph, mutex); });
  std::thread yellow_thread(
      [&graph, &mutex]() { generate_yellow_edges(graph, mutex); });
  std::thread red_thread(
      [&graph, &mutex]() { generate_red_edges(graph, mutex); });
  std::thread blue_thread(
      [&graph, &mutex]() { generate_blue_edges(graph, mutex); });

  green_thread.join();
  yellow_thread.join();
  red_thread.join();
  blue_thread.join();

  return graph;
}
}  // namespace uni_cource_cpp
