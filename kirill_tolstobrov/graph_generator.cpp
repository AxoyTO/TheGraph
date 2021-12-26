#include "graph_generator.hpp"

#include <atomic>
#include <functional>
#include <iostream>
#include <list>
#include <random>
#include <thread>

namespace {

constexpr int MAX_THREADS_COUNT = 4;

constexpr float GREEN_PROB = 0.1;
constexpr float BLUE_PROB = 0.25;
constexpr float RED_PROB = 0.33;

bool random_bool(float true_prob) {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::bernoulli_distribution d(true_prob);
  return d(gen);
}

uni_cpp_practice::VertexId random_vertex_id(
    const std::vector<uni_cpp_practice::VertexId>& vertex_ids) {
  std::random_device dev;
  std::mt19937 rng(dev());
  std::uniform_int_distribution<std::mt19937::result_type> random_index(
      0, vertex_ids.size() - 1);
  return vertex_ids[random_index(rng)];
}

}  // namespace

namespace uni_cpp_practice {
Graph GraphGenerator::generate_random_graph() const {
  Graph graph = Graph();

  std::mutex mutex;

  generate_grey_edges(graph, params_.depth, params_.new_vertices_num);

  std::thread green_thread(
      [&graph, &mutex, this]() { generate_green_edges(graph, mutex); });

  std::thread yellow_thread(
      [&graph, &mutex, this]() { generate_yellow_edges(graph, mutex); });
  std::thread red_thread(
      [&graph, &mutex, this]() { generate_red_edges(graph, mutex); });

  green_thread.join();
  yellow_thread.join();
  red_thread.join();

  return graph;
}

void GraphGenerator::generate_grey_edges(Graph& graph,
                                         int depth,
                                         int new_vertices_num) const {
  using JobCallback = std::function<void()>;
  auto jobs = std::list<JobCallback>();
  std::mutex graph_mutex;

  const VertexId first_vertex_id = graph.add_new_vertex();
  std::atomic<int> done_jobs_number = 0;
  for (int i = 0; i < new_vertices_num; i++) {
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

  const auto threads_count = std::min(MAX_THREADS_COUNT, new_vertices_num);
  auto threads = std::vector<std::thread>();
  threads.reserve(threads_count);

  for (int i = 0; i < threads_count; i++) {
    threads.push_back(std::thread(worker));
  }

  while (done_jobs_number < new_vertices_num) {
  }

  should_terminate = true;
  for (auto& thread : threads) {
    thread.join();
  }
}

void GraphGenerator::generate_gray_branch(Graph& graph,
                                          int depth,
                                          const VertexId& vertex_id,
                                          std::mutex& mutex) const {
  if (depth == params_.depth) {
    return;
  }

  const float new_vertex_prob = 1.0 - (float)depth / params_.depth;
  if (!random_bool(new_vertex_prob)) {
    return;
  }

  const VertexId new_vertex_id = [&mutex, &graph, &vertex_id]() {
    const std::lock_guard lock(mutex);
    const auto new_vertex_id = graph.add_new_vertex();
    graph.bind_vertices(vertex_id, new_vertex_id);
    return new_vertex_id;
  }();

  for (int i = 0; i < params_.new_vertices_num; i++) {
    generate_gray_branch(graph, depth + 1, new_vertex_id, mutex);
  }
}

void GraphGenerator::generate_green_edges(Graph& graph,
                                          std::mutex& mutex) const {
  for (const auto& cur_vertex : graph.get_vertices()) {
    if (random_bool(GREEN_PROB)) {
      const std::lock_guard lock(mutex);
      graph.bind_vertices(cur_vertex.id, cur_vertex.id);
    }
  }
}

void GraphGenerator::generate_yellow_edges(Graph& graph,
                                           std::mutex& mutex) const {
  float yellow_probability = 0;
  const float probability_increasement = 1.0 / (graph.depths_map_.size() - 1);
  for (int cur_depth = 0; cur_depth < graph.depths_map_.size() - 1;
       cur_depth++) {
    const auto& vertex_ids_at_depth = graph.depths_map_[cur_depth];
    const auto& vertex_ids_at_next_depth = graph.depths_map_[cur_depth + 1];
    for (const VertexId cur_id : vertex_ids_at_depth) {
      if (random_bool(yellow_probability)) {
        std::vector<VertexId> possible_connections;
        for (const VertexId next_id : vertex_ids_at_next_depth) {
          if (!graph.are_vertices_connected(cur_id, next_id)) {
            possible_connections.push_back(next_id);
          }
        }
        if (possible_connections.size() > 0) {
          const VertexId binding_id = random_vertex_id(possible_connections);
          const std::lock_guard lock(mutex);
          graph.bind_vertices(cur_id, binding_id);
        }
      }
    }
    yellow_probability += probability_increasement;
  }
}

void GraphGenerator::generate_red_edges(Graph& graph, std::mutex& mutex) const {
  for (int cur_depth = 0; cur_depth < graph.depths_map_.size() - 2;
       cur_depth++) {
    const auto& vertex_ids_at_depth = graph.depths_map_[cur_depth];
    for (const VertexId cur_id : vertex_ids_at_depth) {
      if (random_bool(RED_PROB)) {
        const VertexId binding_id =
            random_vertex_id(graph.depths_map_[cur_depth + 2]);
        const std::lock_guard lock(mutex);
        graph.bind_vertices(cur_id, binding_id);
      }
    }
  }
}
}  // namespace uni_cpp_practice
