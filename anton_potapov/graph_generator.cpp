#include <algorithm>
#include <atomic>
#include <cassert>
#include <functional>
#include <iterator>
#include <limits>
#include <map>
#include <mutex>
#include <queue>
#include <random>
#include <set>
#include <thread>
#include <utility>
#include <vector>

#include "graph.hpp"
#include "graph_generator.hpp"

namespace {
constexpr float FLOAT_COMPARISON_EPS = std::numeric_limits<float>::epsilon();
constexpr float GREEN_EDGE_PROB = 0.1;
constexpr float BLUE_EDGE_PROB = 0.25;
constexpr float RED_EDGE_PROB = 0.33;

bool is_lucky(float probability) {
  assert(probability + FLOAT_COMPARISON_EPS > 0 &&
         probability - FLOAT_COMPARISON_EPS < 1 &&
         "given probability is incorrect");
  static std::knuth_b rand_engine{};
  std::mt19937 rng{rand_engine()};
  std::bernoulli_distribution bernoullu_distribution_var(probability);
  return bernoullu_distribution_var(rng);
}
}  // namespace

namespace uni_cource_cpp {
const int MAX_THREADS_COUNT = std::thread::hardware_concurrency();

void GraphGenerator::generate_vertices_branch(
    Graph& graph,
    std::mutex& graph_mutex,
    const VertexId& previous_vertex_id,
    int current_depth) const {
  if (current_depth > params_.depth || params_.depth == 0) {
    return;
  }
  VertexId new_vertex_id = [&graph_mutex, &graph]() {
    const std::lock_guard graph_lock(graph_mutex);
    return graph.add_vertex();
  }();
  {
    const std::lock_guard graph_lock(graph_mutex);
    graph.add_edge(previous_vertex_id, new_vertex_id);
  }
  for (int i = 0; i < params_.new_vertices_num; ++i) {
    if (is_lucky(1.0 - (float)current_depth / params_.depth)) {
      generate_vertices_branch(graph, graph_mutex, new_vertex_id,
                               current_depth + 1);
    }
  }
}

void GraphGenerator::generate_vertices(Graph& graph) const {
  const VertexId first_vertex_id = graph.add_vertex();
  std::mutex graph_mutex;

  using JobCallback = std::function<void()>;
  auto jobs = std::queue<JobCallback>();

  for (int i = 0; i < params_.new_vertices_num; ++i) {
    jobs.push([&graph, &first_vertex_id, &graph_mutex, this]() {
      generate_vertices_branch(graph, graph_mutex, first_vertex_id, 1);
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
      std::min(MAX_THREADS_COUNT, params_.new_vertices_num);
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
}

void GraphGenerator::generate_green_edges(
    Graph& graph,
    std::mutex& graph_read_write_mutex) const {
  const auto& vertices = [&graph, &graph_read_write_mutex]() {
    const std::lock_guard graph_lock(graph_read_write_mutex);
    return graph.vertices();
  }();
  for (const auto& [vertex_id, vertex] : vertices) {
    if (is_lucky(GREEN_EDGE_PROB)) {
      const std::lock_guard graph_lock(graph_read_write_mutex);
      graph.add_edge(vertex_id, vertex_id, EdgeColor::Green);
    }
  }
}

void GraphGenerator::generate_blue_edges(
    Graph& graph,
    std::mutex& graph_read_write_mutex) const {
  const auto graph_depth = [&graph, &graph_read_write_mutex]() {
    const std::lock_guard graph_lock(graph_read_write_mutex);
    return graph.depth();
  }();
  for (int cur_depth = 0; cur_depth <= graph_depth; ++cur_depth) {
    // copy is needed since get_vertices_at_depth() returns a const reference to
    // a changing object
    const auto same_depth_vertices = [&graph, &graph_read_write_mutex,
                                      &cur_depth]() {
      const std::lock_guard graph_lock(graph_read_write_mutex);
      return graph.get_vertices_at_depth(cur_depth);
    }();
    for (auto it = same_depth_vertices.begin();
         std::next(it) != same_depth_vertices.end(); ++it) {
      const auto& vertex1_id = *it;
      const auto& vertex2_id = *std::next(it);
      const std::lock_guard graph_lock(graph_read_write_mutex);
      if (is_lucky(BLUE_EDGE_PROB) &&
          !graph.is_connected(vertex1_id, vertex2_id)) {
        graph.add_edge(vertex1_id, vertex2_id, EdgeColor::Blue);
      }
    }
  }
}

void GraphGenerator::generate_yellow_edges(
    Graph& graph,
    std::mutex& graph_read_write_mutex) const {
  const auto graph_depth = [&graph, &graph_read_write_mutex]() {
    const std::lock_guard graph_lock(graph_read_write_mutex);
    return graph.depth();
  }();
  if (graph_depth <= 1) {
    return;
  }
  for (int cur_depth = 0; cur_depth + 1 <= graph_depth; ++cur_depth) {
    // copies are needed since get_vertices_at_depth() returns a const reference
    // to a changing object
    const auto get_vertices_at_depth_synch =
        [&graph, &graph_read_write_mutex](int cur_depth) {
          const std::lock_guard graph_lock(graph_read_write_mutex);
          return graph.get_vertices_at_depth(cur_depth);
        };
    const auto cur_depth_vertices = get_vertices_at_depth_synch(cur_depth);
    const auto next_depth_vertices = get_vertices_at_depth_synch(cur_depth + 1);
    for (const auto& cur_vertex_id : cur_depth_vertices) {
      if (is_lucky((float)cur_depth / (graph_depth - 1))) {
        std::set<VertexId> not_connected_vertices;
        const std::lock_guard graph_lock(graph_read_write_mutex);
        for (const auto& next_vertex_id : next_depth_vertices) {
          if (!graph.is_connected(cur_vertex_id, next_vertex_id)) {
            not_connected_vertices.insert(next_vertex_id);
          }
        }
        if (!not_connected_vertices.empty()) {
          graph.add_edge(cur_vertex_id,
                         get_random_vertex_id(not_connected_vertices),
                         EdgeColor::Yellow);
        }
      }
    }
  }
}

void GraphGenerator::generate_red_edges(Graph& graph) const {
  for (int cur_depth = 0; cur_depth + 2 <= graph.depth(); ++cur_depth) {
    // copy is needed since get_vertices_at_depth() returns a const reference to
    // a changing object
    const auto cur_depth_vertices = graph.get_vertices_at_depth(cur_depth);
    for (const auto& cur_vertex_id : cur_depth_vertices) {
      if (cur_depth + 2 > graph.depth()) {
        break;
      }
      // copy is needed since get_vertices_at_depth() returns a const reference
      // to a changing object
      const auto next_depth_vertices =
          graph.get_vertices_at_depth(cur_depth + 2);
      if (is_lucky(RED_EDGE_PROB)) {
        const auto chosen_vertex_id = get_random_vertex_id(next_depth_vertices);
        graph.add_edge(cur_vertex_id, chosen_vertex_id, EdgeColor::Red);
      }
    }
  }
}

GraphGenerator::Params::Params(int depth_val, int new_vertices_num_val)
    : depth(depth_val), new_vertices_num(new_vertices_num_val) {}

GraphGenerator::GraphGenerator(const Params& params) : params_(params) {}

Graph GraphGenerator::generate_graph() const {
  Graph graph;
  generate_vertices(graph);
  std::mutex graph_read_write_mutex;
  std::thread green_edges_generation_thread(
      &GraphGenerator::generate_green_edges, this, std::ref(graph),
      std::ref(graph_read_write_mutex));
  std::thread blue_edges_generation_thread(&GraphGenerator::generate_blue_edges,
                                           this, std::ref(graph),
                                           std::ref(graph_read_write_mutex));
  std::thread yellow_edges_generation_thread(
      &GraphGenerator::generate_yellow_edges, this, std::ref(graph),
      std::ref(graph_read_write_mutex));
  green_edges_generation_thread.join();
  blue_edges_generation_thread.join();
  yellow_edges_generation_thread.join();
  generate_red_edges(graph);
  return graph;
}
}  // namespace uni_cource_cpp
