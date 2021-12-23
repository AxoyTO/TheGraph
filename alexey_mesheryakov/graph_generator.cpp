#include "graph_generator.hpp"
#include <assert.h>
#include <atomic>
#include <functional>
#include <list>
#include <random>
#include <sstream>
#include <string>
#include <thread>
#include <vector>
#include <optional>
#include "graph.hpp"

namespace {
const int MAX_THREADS_COUNT = std::thread::hardware_concurrency();
using std::vector;
using uni_course_cpp::Depth;
using uni_course_cpp::Edge;
using uni_course_cpp::Graph;
using uni_course_cpp::VertexId;

float get_color_probability(const Edge::Color& color,
                            const Depth& current_depth = 0,
                            const Depth& max_depth = 1) {
  switch (color) {
    case Edge::Color::Green:
      return 0.1;
    case Edge::Color::Red:
      return 0.33;
    case Edge::Color::Yellow:
      return static_cast<float>(current_depth) / (max_depth - 1);
    case Edge::Color::Gray:
      return 1.0 - static_cast<float>(current_depth) / (max_depth - 1);
  }
  throw std::runtime_error("Color doesn't exist");
}

bool is_lucky(float probability) {
  assert(probability + std::numeric_limits<float>::epsilon() >= 0 &&
         "given probability is incorrect");
  assert(probability - std::numeric_limits<float>::epsilon() <= 1.0 &&
         "given probability is incorrect");
  static std::knuth_b rand_engine{};
  std::mt19937 rng{rand_engine()};
  std::bernoulli_distribution bernoullu_distribution_var(probability);
  return bernoullu_distribution_var(rng);
}

int get_random_number(int size) {
  std::random_device rd;
  std::default_random_engine gen(rd());
  std::uniform_int_distribution<int> distrib(0, size - 1);
  return distrib(gen);
}

void generate_green_edges(Graph& graph, std::mutex& graph_edges_mutex) {
  for (Depth depth = 1; depth < graph.get_depth(); depth++) {
    for (const auto& vertex_id : graph.get_vertex_ids_at_depth(depth))
      if (is_lucky(get_color_probability(Edge::Color::Green))) {
        const std::lock_guard lock(graph_edges_mutex);
        graph.add_edge(vertex_id, vertex_id);
      }
  }
}

void generate_red_edges(Graph& graph, std::mutex& graph_edges_mutex) {
  for (Depth depth = 1; depth < graph.get_depth() - 2; depth++) {
    for (const auto& vertex_id : graph.get_vertex_ids_at_depth(depth)) {
      if (is_lucky(get_color_probability(Edge::Color::Red))) {
        const std::lock_guard lock(graph_edges_mutex);
        const auto& level_for_red_edge =
            graph.get_vertex_ids_at_depth(depth + 2);
        if (level_for_red_edge.size() > 0)
          graph.add_edge(
              vertex_id,
              level_for_red_edge[get_random_number(level_for_red_edge.size())]);
      }
    }
  }
}

vector<VertexId> get_unconnected_vertex_ids(
    const Graph& graph,
    const VertexId& vertex_id,
    const vector<VertexId>& vertex_ids) {
  vector<VertexId> new_vertices;
  for (const auto& new_vertex_id : vertex_ids)
    if (!graph.edge_exist(vertex_id, new_vertex_id))
      new_vertices.push_back(new_vertex_id);
  return new_vertices;
}

void generate_yellow_edges(Graph& graph, std::mutex& graph_edges_mutex) {
  for (Depth depth = 1; depth < graph.get_depth() - 1; depth++) {
    const auto& level = graph.get_vertex_ids_at_depth(depth);
    for (const auto& vertex_id : level) {
      if (is_lucky(get_color_probability(Edge::Color::Yellow, depth,
                                         graph.get_depth() - 1))) {
        const auto new_vertices = get_unconnected_vertex_ids(
            graph, vertex_id, graph.get_vertex_ids_at_depth(depth + 1));
        if (new_vertices.size() > 0) {
          const std::lock_guard lock(graph_edges_mutex);
          graph.add_edge(vertex_id,
                         new_vertices[get_random_number(new_vertices.size())]);
        }
      }
    }
  }
}

}  // namespace
namespace uni_course_cpp {

void GraphGenerator::generate_gray_edges(Graph& graph) const {
  const VertexId root_vertex_id = graph.add_vertex();
  using JobCallback = std::function<void()>;
  auto jobs = std::list<JobCallback>();
  std::atomic<int> jobs_counter = 0;
  std::mutex graph_edges_mutex;
  for (int i = 0; i < params_.new_vertices_num; i++) {
    jobs.emplace_back(
        [this, &graph, &graph_edges_mutex, &jobs_counter, &root_vertex_id]() {
          generate_gray_branch(graph, root_vertex_id, 1, graph_edges_mutex);
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
  for (auto& worker_thread : threads) {
    worker_thread.join();
  }
}

void GraphGenerator::generate_gray_branch(Graph& graph,
                                          const VertexId& from_vertex_id,
                                          const Depth& depth,
                                          std::mutex& graph_edges_mutex) const {
  if (depth == params_.depth)
    return;

  const auto new_vertex_id = [&graph, &from_vertex_id, &graph_edges_mutex]() {
    const std::lock_guard lock(graph_edges_mutex);
    const auto new_vertex_id = graph.add_vertex();
    graph.add_edge(from_vertex_id, new_vertex_id);
    return new_vertex_id;
  }();

  for (int i = 0; i < params_.new_vertices_num; i++)
    if (is_lucky(
            get_color_probability(Edge::Color::Gray, depth - 1, params_.depth)))
      generate_gray_branch(graph, new_vertex_id, depth + 1, graph_edges_mutex);
}

Graph GraphGenerator::generate_graph() const {
  Graph graph;
  generate_gray_edges(graph);
  std::mutex graph_edges_mutex;

  std::thread green_tread(generate_green_edges, std::ref(graph),
                          std::ref(graph_edges_mutex));
  std::thread yellow_thread(generate_yellow_edges, std::ref(graph),
                            std::ref(graph_edges_mutex));
  std::thread red_thread(generate_red_edges, std::ref(graph),
                         std::ref(graph_edges_mutex));
  green_tread.join();
  yellow_thread.join();
  red_thread.join();
  return graph;
}

}  // namespace uni_course_cpp
