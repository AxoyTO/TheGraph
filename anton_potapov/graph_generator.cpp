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
using uni_cource_cpp::EdgeColor;
using uni_cource_cpp::get_random_vertex_id;
using uni_cource_cpp::Graph;
using uni_cource_cpp::Vertex;
using uni_cource_cpp::VertexId;

constexpr float FLOAT_COMPARISON_EPS = std::numeric_limits<float>::epsilon();
constexpr float GREEN_EDGE_PROB = 0.1;
constexpr float BLUE_EDGE_PROB = 0.25;
constexpr float RED_EDGE_PROB = 0.33;
const int MAX_THREADS_COUNT = std::thread::hardware_concurrency();

bool is_lucky(float probability) {
  assert(probability + FLOAT_COMPARISON_EPS > 0 &&
         probability - FLOAT_COMPARISON_EPS < 1 &&
         "given probability is incorrect");
  static std::knuth_b rand_engine{};
  std::mt19937 rng{rand_engine()};
  std::bernoulli_distribution bernoullu_distribution_var(probability);
  return bernoullu_distribution_var(rng);
}

void generate_vertices_branch(Graph& graph,
                              std::mutex& graph_mutex,
                              int max_depth,
                              int new_vertices_num,
                              const VertexId& previous_vertex_id,
                              int current_depth) {
  if (current_depth > max_depth) {
    return;
  }
  VertexId new_vertex_id;
  {
    const std::lock_guard graph_lock(graph_mutex);
    new_vertex_id = graph.add_vertex();
    graph.add_edge(previous_vertex_id, new_vertex_id);
  }
  for (int i = 0; i < new_vertices_num; ++i) {
    if (max_depth > 0 && is_lucky(1.0 - (float)current_depth / max_depth)) {
      generate_vertices_branch(graph, graph_mutex, max_depth, new_vertices_num,
                               new_vertex_id, current_depth + 1);
    }
  }
}

void generate_vertices(Graph& graph, int depth, int new_vertices_num) {
  const VertexId first_vertex_id = graph.add_vertex();
  std::mutex graph_mutex;

  using JobCallback = std::function<void()>;
  auto jobs = std::queue<JobCallback>();

  for (int i = 0; i < new_vertices_num; ++i) {
    jobs.push(
        [&graph, &depth, &new_vertices_num, &first_vertex_id, &graph_mutex]() {
          generate_vertices_branch(graph, graph_mutex, depth, new_vertices_num,
                                   first_vertex_id, 1);
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

  const auto threads_count = std::min(MAX_THREADS_COUNT, new_vertices_num);
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

void generate_green_edges(Graph& graph, std::mutex& graph_read_write_mutex) {
  graph_read_write_mutex.lock();
  const auto& vertices = graph.vertices();
  graph_read_write_mutex.unlock();
  for (const auto& [vertex_id, vertex] : vertices) {
    if (is_lucky(GREEN_EDGE_PROB)) {
      const std::lock_guard graph_lock(graph_read_write_mutex);
      graph.add_edge(vertex_id, vertex_id, EdgeColor::Green);
    }
  }
}

void generate_blue_edges(Graph& graph, std::mutex& graph_read_write_mutex) {
  graph_read_write_mutex.lock();
  const auto graph_depth = graph.depth();
  graph_read_write_mutex.unlock();
  for (int cur_depth = 0; cur_depth <= graph_depth; ++cur_depth) {
    // copy is needed since get_vertices_at_depth() returns a const reference to
    // a changing object
    graph_read_write_mutex.lock();
    const auto same_depth_vertices = graph.get_vertices_at_depth(cur_depth);
    graph_read_write_mutex.unlock();
    for (auto it = same_depth_vertices.begin();
         std::next(it) != same_depth_vertices.end(); ++it) {
      const auto& vertex1_id = *it;
      const auto& vertex2_id = *std::next(it);
      if (is_lucky(BLUE_EDGE_PROB) &&
          !graph.is_connected(vertex1_id, vertex2_id)) {
        const std::lock_guard graph_lock(graph_read_write_mutex);
        graph.add_edge(vertex1_id, vertex2_id, EdgeColor::Blue);
      }
    }
  }
  graph_read_write_mutex.unlock();
}

void generate_yellow_edges(Graph& graph, std::mutex& graph_read_write_mutex) {
  graph_read_write_mutex.lock();
  const auto graph_depth = graph.depth();
  graph_read_write_mutex.unlock();
  if (graph_depth <= 1) {
    return;
  }
  for (int cur_depth = 0; cur_depth + 1 <= graph_depth; ++cur_depth) {
    // copies are needed since get_vertices_at_depth() returns a const reference
    // to a changing object
    graph_read_write_mutex.lock();
    const auto cur_depth_vertices = graph.get_vertices_at_depth(cur_depth);
    const auto next_depth_vertices = graph.get_vertices_at_depth(cur_depth + 1);
    graph_read_write_mutex.unlock();
    for (const auto& cur_vertex_id : cur_depth_vertices) {
      if (is_lucky((float)cur_depth / (graph_depth - 1))) {
        std::set<VertexId> not_connected_vertices;
        for (const auto& next_vertex_id : next_depth_vertices) {
          if (!graph.is_connected(cur_vertex_id, next_vertex_id)) {
            not_connected_vertices.insert(next_vertex_id);
          }
        }
        if (!not_connected_vertices.empty()) {
          const std::lock_guard graph_lock(graph_read_write_mutex);
          graph.add_edge(cur_vertex_id,
                         get_random_vertex_id(not_connected_vertices),
                         EdgeColor::Yellow);
        }
      }
    }
  }
}

void generate_red_edges(Graph& graph) {
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
}  // namespace

namespace uni_cource_cpp {
GraphGenerator::Params::Params(int depth_val, int new_vertices_num_val)
    : depth(depth_val), new_vertices_num(new_vertices_num_val) {}

GraphGenerator::GraphGenerator(const Params& params) : params_(params) {}

Graph GraphGenerator::generate_graph() const {
  Graph graph;
  generate_vertices(graph, params_.depth, params_.new_vertices_num);
  std::mutex graph_read_write_mutex;
  std::thread green_edges_generation_thread(
      &generate_green_edges, std::ref(graph), std::ref(graph_read_write_mutex));
  std::thread blue_edges_generation_thread(
      &generate_blue_edges, std::ref(graph), std::ref(graph_read_write_mutex));
  std::thread yellow_edges_generation_thread(&generate_yellow_edges,
                                             std::ref(graph),
                                             std::ref(graph_read_write_mutex));
  green_edges_generation_thread.join();
  blue_edges_generation_thread.join();
  yellow_edges_generation_thread.join();
  generate_red_edges(graph);
  return graph;
}
}  // namespace uni_cource_cpp
