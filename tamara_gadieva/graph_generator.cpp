#include "graph_generator.hpp"
#include <array>
#include <atomic>
#include <functional>
#include <list>
#include <random>
#include <stdexcept>
#include <thread>
#include "graph.hpp"

namespace {
constexpr float GREEN_EDGE_PROB = 0.1;
constexpr float RED_EDGE_PROB = 0.33;
const int MAX_THREADS_COUNT = std::thread::hardware_concurrency();

using Graph = uni_cource_cpp::Graph;

bool should_create_new_element(float probability) {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::bernoulli_distribution d(probability);
  return d(gen);
}

const VertexId& get_random_vertex_id(
    const std::vector<VertexId>& vertices_ids) {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<int> d(0, vertices_ids.size() - 1);
  return vertices_ids[d(gen)];
}

const std::vector<VertexId> get_unconnected_vertices(
    const VertexId& id,
    const std::vector<VertexId>& vertices_ids,
    const Graph& graph,
    std::mutex& graph_mutex) {
  std::vector<VertexId> unconnected_vertices;
  for (const auto& vertex_id : vertices_ids) {
    const auto is_connected_bool = [&graph, &id, &vertex_id, &graph_mutex]() {
      const std::lock_guard lock(graph_mutex);
      return graph.is_connected(id, vertex_id);
    }();
    if (!is_connected_bool)
      unconnected_vertices.emplace_back(vertex_id);
  }
  return unconnected_vertices;
}
}  // namespace

namespace uni_cource_cpp {
void GraphGenerator::generate_gray_branch(Graph& graph,
                                          const VertexId& from_vertex_id,
                                          int current_depth,
                                          std::mutex& graph_mutex) const {
  const auto& new_vertex_id = [&graph, &from_vertex_id, &graph_mutex]() {
    const std::lock_guard lock(graph_mutex);
    const auto& to_vertex_id = graph.add_vertex();
    graph.add_edge(from_vertex_id, to_vertex_id);
    return to_vertex_id;
  }();
  if (current_depth == params_.depth)
    return;
  const float prob_of_creating_new_vertex =
      (float)(params_.depth - current_depth) / (float)params_.depth;
  for (int i = 0; i < params_.new_vertices_num; i++)
    if (should_create_new_element(prob_of_creating_new_vertex))
      generate_gray_branch(graph, new_vertex_id, current_depth + 1,
                           graph_mutex);
}

void GraphGenerator::generate_gray_edges(Graph& graph,
                                         const VertexId& from_vertex_id) const {
  using JobCallback = std::function<void()>;
  auto jobs = std::list<JobCallback>();
  std::atomic<int> jobs_counter = 0;
  std::mutex graph_mutex;

  for (int i = 0; i < params_.new_vertices_num; i++) {
    jobs.emplace_back(
        [this, &graph, &graph_mutex, &jobs_counter, &from_vertex_id]() {
          generate_gray_branch(graph, from_vertex_id, 1, graph_mutex);
          jobs_counter++;
        });
  }

  std::mutex jobs_mutex;
  std::atomic<bool> should_terminate = false;
  // Создаем воркера, который в бесконечном цикле проверяет,
  // есть ли работа, и выполняет её
  const auto worker = [&should_terminate, &jobs_mutex, &jobs]() {
    while (true) {
      if (should_terminate)
        return;
      const auto job_optional = [&jobs_mutex,
                                 &jobs]() -> std::optional<JobCallback> {
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

  // Создаем и запускаем потоки с воркерами
  const auto threads_counter =
      std::min(MAX_THREADS_COUNT, params_.new_vertices_num);
  auto threads = std::vector<std::thread>();
  threads.reserve(threads_counter);
  for (int i = 0; i < threads_counter; ++i) {
    threads.push_back(std::thread(worker));
  }
  // Ждем, когда все ветви будут сгенерированы
  while (jobs_counter < params_.new_vertices_num) {
  }

  // Останавливем всех воркеров и потоки
  should_terminate = true;
  for (auto& thread : threads) {
    thread.join();
  }
}

void generate_green_edges(Graph& graph, std::mutex& graph_mutex) {
  for (int depth = 0; depth < graph.get_depth(); depth++)
    for (const auto& vertex_id_in_current_depth :
         graph.get_vertex_ids_in_depth(depth))
      if (should_create_new_element(GREEN_EDGE_PROB)) {
        const std::lock_guard lock(graph_mutex);
        graph.add_edge(vertex_id_in_current_depth, vertex_id_in_current_depth);
      }
}

void generate_yellow_edges(Graph& graph, std::mutex& graph_mutex) {
  for (int depth = 0; depth < graph.get_depth() - 2; depth++) {
    const float prob_of_creating_new_edge =
        (float)depth / (float)(graph.get_depth() - 1);
    const auto& vertices_ids_in_current_depth =
        graph.get_vertex_ids_in_depth(depth);
    const auto& vertices_ids_in_prev_depth =
        graph.get_vertex_ids_in_depth(depth + 1);
    for (const auto& vertex_id_in_current_depth :
         vertices_ids_in_current_depth) {
      if (should_create_new_element(prob_of_creating_new_edge)) {
        auto unconnected_vertices = get_unconnected_vertices(
            vertex_id_in_current_depth, vertices_ids_in_prev_depth, graph,
            graph_mutex);
        if (unconnected_vertices.size() == 0)
          continue;
        const auto& random_vertex_id_in_prev_depth =
            get_random_vertex_id(unconnected_vertices);
        const std::lock_guard lock(graph_mutex);
        graph.add_edge(vertex_id_in_current_depth,
                       random_vertex_id_in_prev_depth);
      }
    }
  }
}

void generate_red_edges(Graph& graph, std::mutex& graph_mutex) {
  for (int depth = 0; depth < graph.get_depth() - 2; depth++) {
    const auto& vertices_ids_in_current_depth =
        graph.get_vertex_ids_in_depth(depth);
    const auto& vertices_ids_in_next_depth =
        graph.get_vertex_ids_in_depth(depth + 2);
    for (const auto& vertex_id_in_current_depth : vertices_ids_in_current_depth)
      if (should_create_new_element(RED_EDGE_PROB)) {
        const std::lock_guard lock(graph_mutex);
        graph.add_edge(vertex_id_in_current_depth,
                       get_random_vertex_id(vertices_ids_in_next_depth));
      }
  }
}

Graph GraphGenerator::generate() const {
  Graph graph;
  if (params_.depth == 0 || params_.new_vertices_num == 0)
    return graph;
  const auto root_id = graph.add_vertex();
  generate_gray_edges(graph, root_id);
  std::mutex graph_mutex;
  std::thread green_tread(generate_green_edges, std::ref(graph),
                          std::ref(graph_mutex));
  std::thread yellow_thread(generate_yellow_edges, std::ref(graph),
                            std::ref(graph_mutex));
  std::thread red_thread(generate_red_edges, std::ref(graph),
                         std::ref(graph_mutex));
  green_tread.join();
  yellow_thread.join();
  red_thread.join();
  return graph;
}
}  // namespace uni_cource_cpp
