#include "graph_generator.hpp"
#include <atomic>
#include <functional>
#include <list>
#include <mutex>
#include <random>
#include <thread>
#include <vector>

namespace {
constexpr float RED_GENERATION_PROBABILITY = 0.33;
constexpr float GREEN_GENERATION_PROBABILITY = 0.1;
const int MAX_THREAD_COUNT = std::thread::hardware_concurrency();

bool checkProbability(float probability) {
  std::random_device seed{};
  std::default_random_engine generator(seed());
  std::bernoulli_distribution generationDistribution(probability);
  return generationDistribution(generator);
}
int randomIntNumber(float maximum) {
  std::random_device seed{};
  std::default_random_engine generator(seed());
  std::uniform_int_distribution<> generationDistribution(0, maximum);
  return generationDistribution(generator);
}

std::vector<uni_course_cpp::VertexId> getUnconnectedVertexIds(
    uni_course_cpp::VertexId vertexId,
    std::vector<uni_course_cpp::VertexId> const& vertexIds,
    uni_course_cpp::Graph const& graph) {
  std::vector<uni_course_cpp::VertexId> notConnected;
  for (auto randomNextVertexId : vertexIds)
    if (!graph.isConnected(vertexId, randomNextVertexId))
      notConnected.emplace_back(randomNextVertexId);
  return notConnected;
};
}  // namespace
namespace uni_course_cpp {
Graph GraphGenerator::generate() const {
  std::mutex mutex;
  auto graph = Graph();
  graph.addVertex();
  generateGrayEdges(graph);
  std::thread greenThread(
      [&graph, &mutex, this]() { generateGreenEdges(graph, mutex); });
  std::thread yellowThread(
      [&graph, &mutex, this]() { generateYellowEdges(graph, mutex); });
  std::thread redThread(
      [&graph, &mutex, this]() { generateRedEdges(graph, mutex); });

  greenThread.join();
  yellowThread.join();
  redThread.join();
  return graph;
}

void GraphGenerator::generateGrayEdges(Graph& graph) const {
  graph.addVertex();
  using JobCallback = std::function<void()>;
  auto jobs = std::list<JobCallback>();
  std::mutex jobsMutex;
  std::atomic<bool> shouldTerminate = false;
  std::atomic<int> jobsDone = 0;
  for (int jobNumber = 0; jobNumber < params_.newVerticesNum; jobNumber++) {
    jobs.push_back([&jobsDone, &jobsMutex, &graph, this]() {
      generateGrayBranch(graph, 0, 0, jobsMutex);
      jobsDone++;
    });
  }
  auto const worker = [&shouldTerminate, &jobsMutex, &jobs]() {
    while (true) {
      if (shouldTerminate) {
        return;
      }
      auto const jobOptional = [&jobs,
                                &jobsMutex]() -> std::optional<JobCallback> {
        std::lock_guard<std::mutex> const lock(jobsMutex);
        if ([&jobs]() { return jobs.size(); }()) {
          return [&jobs]() {
            auto const job = jobs.back();
            jobs.pop_back();
            return job;
          }();
        }
        return std::nullopt;
      }();

      if (jobOptional.has_value()) {
        auto const& job = jobOptional.value();
        job();
      }
    }
  };
  auto const threadCount = std::min(MAX_THREAD_COUNT, params_.newVerticesNum);
  auto threads = std::vector<std::thread>();
  threads.reserve(threadCount);
  for (int i = 0; i < threadCount; ++i) {
    threads.push_back(std::thread(worker));
  }

  while (jobsDone < params_.newVerticesNum) {
  }
  shouldTerminate = true;
  for (auto& thread : threads) {
    thread.join();
  }
}

void GraphGenerator::generateGrayBranch(Graph& graph,
                                        Depth currentDepth,
                                        VertexId const& currentVertexId,
                                        std::mutex& jobsMutex) const {
  if (currentDepth == params_.depth) {
    return;
  }
  if (!checkProbability(float(params_.depth - currentDepth) / params_.depth)) {
    return;
  }

  VertexId const newVertexId = [&graph, &jobsMutex, &currentVertexId]() {
    std::lock_guard<std::mutex> const lock(jobsMutex);
    auto const& newVertex = graph.addVertex();
    graph.addEdge(currentVertexId, newVertex);
    return newVertex;
  }();
  for (int jobNumber = 0; jobNumber < params_.newVerticesNum; ++jobNumber) {
    generateGrayBranch(graph, currentDepth + 1, newVertexId, jobsMutex);
  }
}

void GraphGenerator::generateGreenEdges(Graph& graph, std::mutex& mutex) const {
  for (Graph::Vertex const& vertex : graph.getVertexes()) {
    if (checkProbability(GREEN_GENERATION_PROBABILITY)) {
      std::lock_guard<std::mutex> const lock(mutex);
      graph.addEdge(vertex.id, vertex.id);
    }
  }
}

void GraphGenerator::generateYellowEdges(Graph& graph,
                                         std::mutex& mutex) const {
  float const step = 100.0 / (params_.depth - 1);
  for (auto vertex : graph.getVertexes()) {
    if (checkProbability(
            ((100.0 - (100.0 - (step * (graph.getDepth(vertex.id))))) / 100)) &&
        graph.getDepth(vertex.id) < params_.depth - 1) {
      std::vector<uni_course_cpp::VertexId> nextVertexesIds;
      {
        std::lock_guard<std::mutex> const lock(mutex);
        nextVertexesIds = getUnconnectedVertexIds(
            vertex.id, graph.getVertexIdByDepth(graph.getDepth(vertex.id) + 1),
            graph);
      }
      if (nextVertexesIds.size() > 0) {
        std::lock_guard<std::mutex> const lock(mutex);
        graph.addEdge(
            vertex.id,
            nextVertexesIds[randomIntNumber(nextVertexesIds.size() - 1)]);
      }
    }
  }
}

void GraphGenerator::generateRedEdges(Graph& graph, std::mutex& mutex) const {
  for (auto vertex : graph.getVertexes()) {
    if (graph.getDepth(vertex.id) < params_.depth - 2 &&
        checkProbability(RED_GENERATION_PROBABILITY)) {
      std::vector<uni_course_cpp::VertexId> nextVertexesIds;
      {
        std::lock_guard<std::mutex> const lock(mutex);
        nextVertexesIds = getUnconnectedVertexIds(
            vertex.id, graph.getVertexIdByDepth(graph.getDepth(vertex.id) + 2),
            graph);
      }
      if (nextVertexesIds.size() > 0) {
        std::lock_guard<std::mutex> const lock(mutex);
        graph.addEdge(
            vertex.id,
            nextVertexesIds[randomIntNumber(nextVertexesIds.size() - 1)]);
      }
    }
  }
}
}  // namespace uni_course_cpp
