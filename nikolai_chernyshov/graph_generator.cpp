#include "graph_generator.hpp"
#include <atomic>
#include <functional>
#include <list>
#include <optional>
#include <random>
#include <thread>

namespace {
using VertexId = uni_course_cpp::VertexId;
using Graph = uni_course_cpp::Graph;

constexpr double GREEN_EDGE_PROBA = 0.1;
constexpr double RED_EDGE_PROBA = 0.33;
const int MAX_THREADS_COUNT = std::thread::hardware_concurrency();

bool get_random_boolean(double proba) {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::bernoulli_distribution random_boolean(proba);
  return random_boolean(gen);
}

const VertexId& get_random_vertex_id(const std::vector<VertexId>& vertex_ids) {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> random_vertex(0, vertex_ids.size() - 1);
  return vertex_ids[random_vertex(gen)];
}
}  // namespace

namespace uni_course_cpp {
Graph GraphGenerator::generate() const {
  Graph graph;
  std::mutex mutex;

  generate_gray_edges(graph);

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

void GraphGenerator::generate_gray_edges(Graph& graph) const {
  using JobCallback = std::function<void()>;
  auto jobs = std::list<JobCallback>();

  std::mutex jobs_mutex;
  std::atomic<int> finished_jobs_num = 0;
  std::atomic<bool> should_terminate = false;

  const VertexId root_vertex_id = graph.add_vertex();

  for (int i = 0; i < params_.new_vertices_count; i++) {
    jobs.push_back(
        [&graph, &jobs_mutex, &root_vertex_id, &finished_jobs_num, this]() {
          generate_gray_branch(graph, 0, root_vertex_id, jobs_mutex);
          finished_jobs_num++;
        });
  }

  auto worker = [&should_terminate, &jobs_mutex, &jobs]() {
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
      std::min(MAX_THREADS_COUNT, params_.new_vertices_count);
  auto threads = std::vector<std::thread>();
  threads.reserve(threads_count);

  for (int i = 0; i < threads_count; i++) {
    threads.push_back(std::thread(worker));
  }

  while (finished_jobs_num < params_.new_vertices_count) {
  }

  should_terminate = true;
  for (auto& thread : threads) {
    thread.join();
  }
}

void GraphGenerator::generate_gray_branch(Graph& graph,
                                          int depth,
                                          const VertexId& parent_vertex_id,
                                          std::mutex& mutex) const {
  if (depth == params_.depth) {
    return;
  }

  const double probability = 1 - (double)depth / (double)params_.depth;
  if (!get_random_boolean(probability)) {
    return;
  }

  const VertexId new_vertex_id = [&mutex, &graph, &parent_vertex_id]() {
    const std::lock_guard lock(mutex);
    const auto new_vertex_id = graph.add_vertex();
    graph.add_edge(parent_vertex_id, new_vertex_id);
    return new_vertex_id;
  }();

  for (int i = 0; i < params_.new_vertices_count; i++) {
    generate_gray_branch(graph, depth + 1, new_vertex_id, mutex);
  }
}

void GraphGenerator::generate_green_edges(Graph& graph,
                                          std::mutex& mutex) const {
  for (const auto& vertex : graph.get_vertices())
    if (get_random_boolean(GREEN_EDGE_PROBA)) {
      const std::lock_guard lock(mutex);
      graph.add_edge(vertex.id, vertex.id);
    }
}

void GraphGenerator::generate_yellow_edges(Graph& graph,
                                           std::mutex& mutex) const {
  // поставил 2 вместо 1, потому что с глубины 0 желтая грань не может
  // начинаться иначе она будет идти к потомку корневой вершины, что
  // противоречит условию работало бы и в случае 1, но тогда производились бы
  // лишние вычисления
  if (graph.get_depth() < 2)
    return;

  double proba_step = 1.0 / (double)(graph.get_depth() - 1);
  const auto& depth_map = graph.get_depth_map();

  for (auto vertex_ids_in_depth = depth_map.begin();
       vertex_ids_in_depth != depth_map.end() - 1; vertex_ids_in_depth++) {
    for (const auto& vertex_id : *vertex_ids_in_depth) {
      std::vector<VertexId> unconnected_vertex_ids;
      for (const auto& vertex_id_in_next_depth : *(vertex_ids_in_depth + 1)) {
        const auto are_connected = [&vertex_id, &vertex_id_in_next_depth,
                                    &mutex, &graph]() {
          const std::lock_guard lock(mutex);
          return graph.are_connected(vertex_id, vertex_id_in_next_depth);
        }();
        if (!are_connected) {
          unconnected_vertex_ids.push_back(vertex_id_in_next_depth);
        }
      }
      if (unconnected_vertex_ids.size() &&
          get_random_boolean(
              proba_step * (double)(vertex_ids_in_depth - depth_map.begin()))) {
        const std::lock_guard lock(mutex);
        graph.add_edge(vertex_id, get_random_vertex_id(unconnected_vertex_ids));
      }
    }
  }
}

void GraphGenerator::generate_red_edges(Graph& graph, std::mutex& mutex) const {
  if (graph.get_depth() < 2)
    return;

  for (auto vertex_ids_in_depth = graph.get_depth_map().begin();
       vertex_ids_in_depth != graph.get_depth_map().end() - 2;
       vertex_ids_in_depth++)
    for (const auto& vertex_id : *vertex_ids_in_depth)
      if (get_random_boolean(RED_EDGE_PROBA)) {
        const VertexId new_id =
            get_random_vertex_id(*(vertex_ids_in_depth + 2));
        const std::lock_guard lock(mutex);
        graph.add_edge(vertex_id, new_id);
      }
}
}  // namespace uni_course_cpp
