#include <cassert>
#include <functional>
#include <iterator>
#include <limits>
#include <map>
#include <random>
#include <set>
#include <thread>
#include <utility>

#include "graph.hpp"
#include "graph_generator.hpp"

namespace {
using uni_cource_cpp::EdgeColor;
using uni_cource_cpp::get_random_vertex_id;
using uni_cource_cpp::Graph;
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

void generate_vertices(Graph& graph, int depth, int new_vertices_num) {
  graph.add_vertex();
  for (int current_depth = 0;
       current_depth <= graph.depth() && current_depth < depth;
       ++current_depth) {
    // copy is needed since get_vertices_at_depth() returns a const reference to
    // a changing object
    const auto same_depth_vertices = graph.get_vertices_at_depth(current_depth);
    for (const auto& current_vertex_id : same_depth_vertices) {
      for (int i = 0; i < new_vertices_num; ++i) {
        if (depth > 0 && is_lucky(1.0 - (float)current_depth / depth)) {
          const VertexId new_vertex_id = graph.add_vertex();
          graph.add_edge(current_vertex_id, new_vertex_id);
        }
      }
    }
  }
}

void generate_green_edges(Graph& graph, std::mutex& graph_read_write_mutex) {
  graph_read_write_mutex.lock();
  for (const auto& [vertex_id, vertex] : graph.vertices()) {
    if (is_lucky(GREEN_EDGE_PROB)) {
      graph.add_edge(vertex_id, vertex_id, EdgeColor::Green);
    }
  }
  graph_read_write_mutex.unlock();
}

void generate_blue_edges(Graph& graph, std::mutex& graph_read_write_mutex) {
  graph_read_write_mutex.lock();
  for (int cur_depth = 0; cur_depth <= graph.depth(); ++cur_depth) {
    // copy is needed since get_vertices_at_depth() returns a const reference to
    // a changing object
    const auto same_depth_vertices = graph.get_vertices_at_depth(cur_depth);
    for (auto it = same_depth_vertices.begin();
         std::next(it) != same_depth_vertices.end(); ++it) {
      const auto& vertex1_id = *it;
      const auto& vertex2_id = *std::next(it);
      if (is_lucky(BLUE_EDGE_PROB) &&
          !graph.is_connected(vertex1_id, vertex2_id)) {
        graph.add_edge(vertex1_id, vertex2_id, EdgeColor::Blue);
      }
    }
  }
  graph_read_write_mutex.unlock();
}

void generate_yellow_edges(Graph& graph, std::mutex& graph_read_write_mutex) {
  graph_read_write_mutex.lock();
  if (graph.depth() <= 1) {
    return;
  }
  for (int cur_depth = 0; cur_depth + 1 <= graph.depth(); ++cur_depth) {
    // copies are needed since get_vertices_at_depth() returns a const reference
    // to a changing object
    const auto cur_depth_vertices = graph.get_vertices_at_depth(cur_depth);
    const auto next_depth_vertices = graph.get_vertices_at_depth(cur_depth + 1);
    for (const auto& cur_vertex_id : cur_depth_vertices) {
      if (is_lucky((float)cur_depth / (graph.depth() - 1))) {
        std::set<VertexId> not_connected_vertices;
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
  graph_read_write_mutex.unlock();
}

void generate_red_edges(Graph& graph, std::mutex& graph_read_write_mutex) {
  graph_read_write_mutex.lock();
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
  graph_read_write_mutex.unlock();
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
  std::thread red_edges_generation_thread(&generate_red_edges, std::ref(graph),
                                          std::ref(graph_read_write_mutex));
  green_edges_generation_thread.join();
  blue_edges_generation_thread.join();
  yellow_edges_generation_thread.join();
  red_edges_generation_thread.join();
  return graph;
}
}  // namespace uni_cource_cpp
