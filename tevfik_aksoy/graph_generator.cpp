#include "graph_generator.hpp"
#include <atomic>
#include <iostream>
#include <mutex>
#include <random>

using VertexId = uni_cpp_practice::VertexId;
using Graph = uni_cpp_practice::Graph;

namespace {
constexpr float GREEN_EDGE_PROBABILITY = 0.1;
constexpr float BLUE_EDGE_PROBABILITY = 0.25;
constexpr float RED_EDGE_PROBABILITY = 0.33;
constexpr int MAX_THREADS_COUNT = 4;

float get_random_probability() {
  std::random_device rd;
  std::mt19937 mt(rd());
  std::uniform_real_distribution<float> probability(0.0, 1);
  return probability(mt);
}

VertexId get_random_vertex_id(const std::vector<VertexId>& vertices) {
  std::random_device rd;
  std::mt19937 mt(rd());
  std::uniform_int_distribution<int> random_vertex_distribution(
      0, vertices.size() - 1);
  return vertices[random_vertex_distribution(mt)];
}

std::vector<VertexId> filter_connected_vertices(
    const VertexId& id,
    const std::vector<VertexId>& vertex_ids,
    const Graph& graph) {
  std::vector<VertexId> result;
  for (const auto& vertex_id : vertex_ids) {
    if (!graph.are_vertices_connected(id, vertex_id)) {
      result.push_back(vertex_id);
    }
  }
  return result;
}
}  // namespace

namespace uni_cpp_practice {

void generate_gray_branch() {
  std::cout << "grbr\n";
  // if(current_depth == max_depth)
  //   return;
  // рекурсивно вызывает сам себя
  // generate_gray_branch(...);
}

void GraphGenerator::generate_vertices_and_gray_edges(Graph& graph) const {
  // Job - это lambda функция,
  // которая энкапсулирует в себе генерацию одной ветви
  using JobCallback = std::function<void()>;
  auto jobs = std::list<JobCallback>();

  // std::mutex mutex;

  enum class State { Idle, Working, ShouldTerminate };
  State state = State::Idle;

  const auto threads_count = params_.new_vertices_num >= MAX_THREADS_COUNT
                                 ? MAX_THREADS_COUNT
                                 : params_.new_vertices_num;

  // Заполняем список работ для воркеров
  std::atomic<int> jobs_count = 0;
  {
    for (int i = 0; i < threads_count; i++) {
      jobs.push_back([]() { generate_gray_branch(); });
      ++jobs_count;
    }
  }
  std::cout << "\n----\n";

  // Создаем воркера,
  // который в бесконечном цикле проверяет,
  // есть ли работа, и выполняет её

  auto worker = [&state, &jobs]() {
    while (true) {
      // Проверка флага, должны ли мы остановить поток
      if (state == State::ShouldTerminate) {
        state = State::Idle;
        return;
      }
      // Проверяем, есть ли для нас работа
      const auto job_optional = [&jobs]() -> std::optional<JobCallback> {
        if (jobs.empty()) {
          return std::nullopt;
        }
        const auto first_job = jobs.front();
        jobs.pop_front();
        return first_job;
      }();
      if (job_optional.has_value()) {
        // Работа есть, выполняем её
        const auto& job = job_optional.value();
        job();
      }
    }
  };
  // Создаем и запускаем потоки с воркерами
  // MAX_THREADS_COUNT = 4
  auto threads = std::array<std::thread, MAX_THREADS_COUNT>();
  std::cout << "A\n";
  state = State::Working;
  worker();
  std::cout << "B\n";
  // Ждем, когда все ветви будут сгенерированы
  while (jobs_count < 3) {
  }
  // Останавливем всех воркеров и потоки
  state = State::ShouldTerminate;
  for (auto& thread : threads) {
    thread.join();
  }

  for (VertexDepth depth = 0; depth < params_.max_depth; depth++) {
    bool is_new_vertex_generated = false;
    const float probability = (float)depth / (float)params_.max_depth;
    for (const auto& source : graph.get_vertices_in_depth(depth)) {
      for (int j = 0; j < params_.new_vertices_num; j++) {
        if (get_random_probability() > probability) {
          is_new_vertex_generated = true;
          const VertexId new_vertex = graph.insert_vertex();
          graph.insert_edge(source, new_vertex);
        }
      }
    }
    if (!is_new_vertex_generated)
      break;
  }

  if (params_.max_depth != graph.depth()) {
    std::cout << "Max depth couldn't be reached. Depth of final vertex: "
              << graph.depth() << "\n";
  }
}

void GraphGenerator::generate_green_edges(Graph& graph) const {
  for (const auto& vertex : graph.get_vertices()) {
    if (get_random_probability() < GREEN_EDGE_PROBABILITY) {
      graph.insert_edge(vertex.id, vertex.id);
    }
  }
}

void GraphGenerator::generate_blue_edges(Graph& graph) const {
  for (int depth = 0; depth < graph.depth(); depth++) {
    const auto& vertices_in_depth = graph.get_vertices_in_depth(depth);
    for (VertexId j = 0; j < vertices_in_depth.size() - 1; j++) {
      const auto source = vertices_in_depth[j];
      const auto destination = vertices_in_depth[j + 1];
      if (get_random_probability() < BLUE_EDGE_PROBABILITY) {
        graph.insert_edge(source, destination);
      }
    }
  }
}

void GraphGenerator::generate_yellow_edges(Graph& graph) const {
  for (VertexDepth depth = 0; depth < graph.depth(); depth++) {
    float probability = 1 - (float)depth * (1 / (float)(graph.depth() - 1));
    const auto& vertices = graph.get_vertices_in_depth(depth);
    const auto& vertices_next = graph.get_vertices_in_depth(depth + 1);
    for (const auto& vertex_id : vertices) {
      if (get_random_probability() > probability) {
        const auto& filtered_vertex_ids =
            filter_connected_vertices(vertex_id, vertices_next, graph);
        if (filtered_vertex_ids.size() == 0) {
          continue;
        }
        VertexId random_vertex_id = get_random_vertex_id(filtered_vertex_ids);
        graph.insert_edge(vertex_id, random_vertex_id);
      }
    }
  }
}

void GraphGenerator::generate_red_edges(Graph& graph) const {
  for (VertexDepth depth = 0; depth < graph.depth() - 1; depth++) {
    const auto& vertices = graph.get_vertices_in_depth(depth);
    const auto& vertices_next = graph.get_vertices_in_depth(depth + 2);
    for (const auto& vertex : vertices) {
      if (get_random_probability() < RED_EDGE_PROBABILITY) {
        graph.insert_edge(vertex, get_random_vertex_id(vertices_next));
      }
    }
  }
}

Graph GraphGenerator::generate() const {
  Graph graph;
  graph.insert_vertex();
  generate_vertices_and_gray_edges(graph);
  generate_green_edges(graph);
  generate_blue_edges(graph);
  generate_yellow_edges(graph);
  generate_red_edges(graph);

  // green_thread.join();
  // yellow_thread.join();
  // red_thread.join();

  return graph;
}
}  // namespace uni_cpp_practice
