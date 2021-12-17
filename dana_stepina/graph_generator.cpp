#include "graph_generator.hpp"

#include <atomic>
#include <cmath>
#include <functional>
#include <iostream>
#include <list>
#include <random>
#include <thread>

using Probability = int;

constexpr int MAX_PROBABILITY = 100, GREEN_PROBABILITY = 10,
              RED_PROBABILITY = 33;
const int MAX_THREADS_COUNT = std::thread::hardware_concurrency();

namespace {
using Graph = uni_cource_cpp::Graph;
using Params = uni_cource_cpp::GraphGenerator::Params;

Probability get_probability(const Depth& depth, const Depth& current_depth) {
  return floor(MAX_PROBABILITY * (depth - current_depth) / depth);
}

bool is_lucky(const Probability& probability) {
  std::random_device rand;
  std::mt19937 generator(rand());
  std::bernoulli_distribution distribution(probability * 0.01);
  return distribution(generator);
}

VertexId choose_random_vertex_id(const std::vector<VertexId>& vertex_ids) {
  std::random_device rand;
  std::default_random_engine generator(rand());
  std::uniform_int_distribution<int> distribution(0, vertex_ids.size() - 1);
  return vertex_ids[distribution(generator)];
}

std::vector<VertexId> get_unconnected_vertex_ids(
    const VertexId& from_vertex_id,
    const std::vector<VertexId>& vertex_ids,
    const Graph& graph) {
  std::vector<VertexId> unconnected_vertex_ids;
  for (const auto& vertex_id : vertex_ids)
    if (!graph.is_connected(from_vertex_id, vertex_id))
      unconnected_vertex_ids.push_back(vertex_id);
  return unconnected_vertex_ids;
}

void generate_green_edges(Graph& graph, std::mutex& mutex) {
  const auto& depth_map = graph.get_depth_map();

  for (const auto& vertex_ids : depth_map)
    for (const auto& vertex_id : vertex_ids)
      if (is_lucky(GREEN_PROBABILITY)) {
        const std::lock_guard lock(mutex);
        graph.add_edge(vertex_id, vertex_id);
      }
}

void generate_yellow_edges(Graph& graph, std::mutex& mutex) {
  const auto& depth_map = graph.get_depth_map();

  for (Depth depth = 0; depth < depth_map.size() - 1; depth++) {
    const auto probability =
        MAX_PROBABILITY - get_probability(depth_map.size() - 2, depth);

    for (const auto& from_vertex_id : depth_map[depth]) {
      if (is_lucky(probability)) {
        const std::vector<VertexId>& vertex_ids = get_unconnected_vertex_ids(
            from_vertex_id, depth_map[depth + 1], graph);
        if (vertex_ids.size() == 0)
          break;
        const std::lock_guard lock(mutex);
        graph.add_edge(from_vertex_id, choose_random_vertex_id(vertex_ids));
      }
    }
  }
}

void generate_red_edges(Graph& graph, std::mutex& mutex) {
  const auto& depth_map = graph.get_depth_map();

  for (Depth depth = 0; depth < depth_map.size() - 2; depth++)
    for (const auto& from_vertex_id : depth_map[depth]) {
      if (is_lucky(RED_PROBABILITY)) {
        const std::vector<VertexId> vertex_ids = depth_map[depth + 2];
        const std::lock_guard lock(mutex);
        graph.add_edge(from_vertex_id,
                       choose_random_vertex_id(depth_map[depth + 2]));
      }
    }
}
}  // namespace

namespace uni_cource_cpp {
void GraphGenerator::generate_grey_edges(Graph& graph) const {
  using JobCallback = std::function<void()>;
  auto jobs = std::list<JobCallback>();

  std::mutex jobs_mutex;
  std::atomic<int> finished_jobs_num = 0;
  std::atomic<bool> should_terminate = false;

  const VertexId zero_vertex_id = graph.add_vertex().id;  // Zero vertex

  for (int i = 0; i < params_.new_vertices_num; i++)
    jobs.push_back(
        [&graph, &jobs_mutex, &zero_vertex_id, &finished_jobs_num, this]() {
          generate_gray_branch(graph, 0, zero_vertex_id, jobs_mutex);
          finished_jobs_num++;
        });

  auto worker = [&should_terminate, &jobs_mutex, &jobs]() {
    while (true) {
      if (should_terminate)
        return;
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
      std::min(MAX_THREADS_COUNT, params_.new_vertices_num);
  auto threads = std::vector<std::thread>();
  threads.reserve(threads_count);

  for (int i = 0; i < threads_count; i++)
    threads.push_back(std::thread(worker));

  while (finished_jobs_num < params_.new_vertices_num) {
  }

  should_terminate = true;
  for (auto& thread : threads)
    thread.join();

  if (graph.get_depth_map().size() - 1 < params_.depth)
    std::cout
        << "The resulting depth of the generated graph is less than expected.\n"
        << "Expected depth: " << params_.depth
        << "\nThe resulting depth: " << graph.get_depth_map().size() - 1
        << std::endl;
}

void GraphGenerator::generate_gray_branch(Graph& graph,
                                          Depth depth,
                                          const VertexId& from_vertex_id,
                                          std::mutex& mutex) const {
  if (depth == params_.depth)
    return;
  const auto& probability = get_probability(params_.depth, depth);

  if (!is_lucky(probability))
    return;

  const VertexId new_vertex_id = [&mutex, &graph, &from_vertex_id]() {
    const std::lock_guard lock(mutex);
    const auto& new_vertex = graph.add_vertex();
    graph.add_edge(from_vertex_id, new_vertex.id);
    return new_vertex.id;
  }();

  for (int i = 0; i < params_.new_vertices_num; i++) {
    generate_gray_branch(graph, depth + 1, new_vertex_id, mutex);
  }
}

Graph GraphGenerator::generate() const {
  auto graph = Graph();
  std::mutex mutex;

  generate_grey_edges(graph);

  std::thread green_thread(
      [&graph, &mutex]() { generate_green_edges(graph, mutex); });
  std::thread yellow_thread(
      [&graph, &mutex]() { generate_yellow_edges(graph, mutex); });
  std::thread red_thread(
      [&graph, &mutex]() { generate_red_edges(graph, mutex); });

  green_thread.join();
  yellow_thread.join();
  red_thread.join();

  return graph;
}
}  // namespace uni_cource_cpp
