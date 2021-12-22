#include "graph_generator.hpp"
#include <atomic>
#include <functional>
#include <list>
#include <mutex>
#include <optional>
#include <random>
#include <thread>
#include "graph.hpp"

namespace {

using std::lock_guard;
using std::mutex;
using std::ref;
using std::thread;
using std::vector;
using uni_cource_cpp::Graph;
constexpr double kGreenEdgesProbability = 10.0;
constexpr double kRedEdgesProbability = 33.0;
const int kMaxThreadsCount = std::thread::hardware_concurrency();

bool is_generated(double probability) {
  std::random_device rd;
  std::default_random_engine gen(rd());
  std::bernoulli_distribution is_gen(probability * 0.01);
  return is_gen(gen);
}

const uni_cource_cpp::Graph::VertexId& get_random_vertex_id(
    const vector<uni_cource_cpp::Graph::VertexId>& vertices) {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> random_id(0, vertices.size() - 1);
  return vertices[random_id(gen)];
}

void generate_green_edges(uni_cource_cpp::Graph& graph, mutex& new_edge_mutex) {
  for (const auto& [vertex_id, vertex] : graph.get_vertices()) {
    if (is_generated(kGreenEdgesProbability)) {
      const lock_guard lock(new_edge_mutex);
      graph.add_edge(vertex_id, vertex_id);
    }
  }
}

void generate_yellow_edges(uni_cource_cpp::Graph& graph,
                           mutex& new_edge_mutex) {
  for (int current_depth = 0; current_depth <= graph.depth(); current_depth++) {
    if (current_depth != graph.depth()) {
      for (const auto& from_vertex_id :
           graph.get_vertex_ids_on_depth(current_depth)) {
        const double probability =
            (double)graph.get_vertex(from_vertex_id).depth *
            (100 / (double)(graph.depth() - 1));
        if (is_generated(probability)) {
          vector<uni_cource_cpp::Graph::VertexId> vertices_on_deeper_depth;
          for (const auto& to_vertex_id :
               graph.get_vertex_ids_on_depth(current_depth + 1)) {
            const bool is_connected = [&graph, &new_edge_mutex, &from_vertex_id,
                                       &to_vertex_id]() {
              const lock_guard lock(new_edge_mutex);
              return graph.is_connected(from_vertex_id, to_vertex_id);
            }();

            if (!is_connected) {
              vertices_on_deeper_depth.push_back(to_vertex_id);
            }
          }

          if (!vertices_on_deeper_depth.empty()) {
            const lock_guard lock(new_edge_mutex);
            graph.add_edge(from_vertex_id,
                           get_random_vertex_id(vertices_on_deeper_depth));
          }
        }
      }
    }
  }
}

void generate_red_edges(uni_cource_cpp::Graph& graph, mutex& new_edge_mutex) {
  for (int current_depth = 0; current_depth <= graph.depth(); current_depth++) {
    if (current_depth >= graph.depth() - 1) {
      break;
    } else {
      for (const auto& from_vertex_id :
           graph.get_vertex_ids_on_depth(current_depth)) {
        if (is_generated(kRedEdgesProbability)) {
          const lock_guard lock(new_edge_mutex);
          graph.add_edge(from_vertex_id,
                         get_random_vertex_id(
                             graph.get_vertex_ids_on_depth(current_depth + 2)));
        }
      }
    }
  }
}

}  // namespace

namespace uni_cource_cpp {

void GraphGenerator::generate_gray_branch(
    Graph& graph,
    const Graph::VertexId& parent_vertex_id,
    mutex& new_edge_mutex,
    const Graph::Depth& depth) const {
  const auto new_vertex_id = [&graph, &new_edge_mutex, &parent_vertex_id]() {
    const lock_guard lock(new_edge_mutex);
    const auto new_vertex_id = graph.add_vertex();
    graph.add_edge(parent_vertex_id, new_vertex_id);
    return new_vertex_id;
  }();

  if (depth == params_.depth())
    return;

  const double step = 100 / (double)params_.depth();
  const double probability = 100 - step * depth;

  for (int i = 0; i < params_.new_vertices_count(); i++) {
    if (is_generated(probability)) {
      generate_gray_branch(graph, new_vertex_id, new_edge_mutex, depth + 1);
    }
  }
}

void GraphGenerator::generate_gray_edges(
    Graph& graph,
    const Graph::VertexId& root_vertex_id) const {
  using JobCallback = std::function<void()>;
  auto jobs = std::list<JobCallback>();
  std::atomic<int> jobs_done = 0;
  mutex new_edge_mutex;

  for (int i = 0; i < params_.new_vertices_count(); i++) {
    jobs.push_back(
        [this, &graph, &new_edge_mutex, &jobs_done, root_vertex_id]() {
          generate_gray_branch(graph, root_vertex_id, new_edge_mutex, 1);
          jobs_done++;
        });
  }
  std::atomic<bool> should_terminate = false;
  mutex jobs_mutex;
  const auto worker = [&should_terminate, &jobs_mutex, &jobs]() {
    while (true) {
      if (should_terminate) {
        return;
      }
      const auto job_optional = [&jobs_mutex,
                                 &jobs]() -> std::optional<JobCallback> {
        const lock_guard lock(jobs_mutex);
        if (!jobs.empty()) {
          const auto next_job = jobs.front();
          jobs.pop_front();
          return next_job;
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
      std::min(kMaxThreadsCount, params_.new_vertices_count());
  auto threads = vector<thread>();
  threads.reserve(threads_count);

  for (int i = 0; i < threads_count; i++) {
    threads.push_back(thread(worker));
  }

  while (jobs_done != params_.new_vertices_count()) {
  }

  should_terminate = true;
  for (auto& thread : threads) {
    thread.join();
  }
}

Graph GraphGenerator::generate() const {
  auto graph = Graph();

  const auto root_vertex_id = graph.add_vertex();

  generate_gray_edges(graph, root_vertex_id);

  mutex new_edge_mutex;

  thread green_thread(generate_green_edges, ref(graph), ref(new_edge_mutex));
  thread yellow_thread(generate_yellow_edges, ref(graph), ref(new_edge_mutex));
  thread red_thread(generate_red_edges, ref(graph), ref(new_edge_mutex));

  green_thread.join();
  yellow_thread.join();
  red_thread.join();

  return graph;
}

}  // namespace uni_cource_cpp
