#include "GraphGenerator.hpp"
#include <algorithm>
#include <atomic>
#include <cassert>
#include <cstdlib>
#include <functional>
#include <iostream>
#include <list>
#include <mutex>
#include <optional>
#include <random>
#include <thread>
#include <vector>

namespace {
constexpr float GREEN_EDGE_PROBABILITY = 0.10;
constexpr float BLUE_EDGE_PROBABILITY = 0.25;
constexpr float RED_EDGE_PROBABILITY = 0.33;
const int MAX_THREADS_COUNT = std::thread::hardware_concurrency();
float getProbabilityGray(int depth, int maxDepth) {
  assert(depth >= 0);
  const float result = 1.0 - (float)depth / (float)maxDepth;
  if (std::isnan(result)) {
    return 0.0;
  }
  return result;
}
int getRandomVertexId(const std::vector<int> destinationLevelIds) {
  assert(!destinationLevelIds.empty() && "destinationLevelIds is empty!!!");
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<std::mt19937::result_type> vertexIndex(
      0, destinationLevelIds.size() - 1);
  return destinationLevelIds[vertexIndex(gen)];
}
bool isLucky(float probability) {
  assert(probability >= 0);
  assert(probability <= 1.0);
  std::random_device rd;
  std::mt19937 gen(rd());
  std::bernoulli_distribution distribution(probability);
  return distribution(gen);
}
void generateGreen(uni_course_cpp::Graph& graph, std::mutex& mutex) {
  for (const auto& vertex : graph.getVertices()) {
    if (isLucky(GREEN_EDGE_PROBABILITY)) {
      const std::lock_guard lock(mutex);
      graph.addEdge(vertex.id, vertex.id, uni_course_cpp::Edge::Color::Green);
    }
  }
}

void generateRed(uni_course_cpp::Graph& graph, std::mutex& mutex) {
  int maxDepth = graph.getDepth();
  for (int depth = 0; depth < maxDepth - 1; depth++) {
    const auto presentDepth = graph.getVertexIdsAtDepth(depth);
    const auto destinationDepth = graph.getVertexIdsAtDepth(depth + 2);
    for (const auto& fromVertexId : presentDepth) {
      if (isLucky(RED_EDGE_PROBABILITY)) {
        if (!destinationDepth.empty()) {
          const auto randomVertexId = getRandomVertexId(destinationDepth);
          const std::lock_guard lock(mutex);
          graph.addEdge(fromVertexId, randomVertexId,
                        uni_course_cpp::Edge::Color::Red);
        }
      }
    }
  }
}

void generateBlue(uni_course_cpp::Graph& graph, std::mutex& mutex) {
  int maxDepth = graph.getDepth();
  for (int depth = 1; depth < maxDepth; depth++) {
    const auto presentDepth = graph.getVertexIdsAtDepth(depth);
    for (auto vertexIt = presentDepth.begin();
         vertexIt < presentDepth.end() - 1; vertexIt++) {
      if (isLucky(BLUE_EDGE_PROBABILITY)) {
        const std::lock_guard lock(mutex);
        graph.addEdge(*vertexIt, *(vertexIt + 1),
                      uni_course_cpp::Edge::Color::Blue);
      }
    }
  }
}
float getProbabilityYellow(int depth, int maxDepth) {
  assert(depth >= 0);
  const float result = (float)depth / (float)(maxDepth - 1);
  if (std::isnan(result)) {
    return 0.0;
  }
  return result;
}
std::vector<int> getUnconnectedVertexIds(
    const int fromVertexId,
    const std::vector<int>& destinationLevel,
    uni_course_cpp::Graph& graph) {
  std::vector<int> unconnectedVertexIds;
  for (const auto& vertexId : destinationLevel) {
    if (!graph.isConnected(fromVertexId, vertexId)) {
      unconnectedVertexIds.push_back(vertexId);
    }
  }
  return unconnectedVertexIds;
}
void generateYellow(uni_course_cpp::Graph& graph, std::mutex& mutex) {
  const int maxDepth = graph.getDepth();
  for (int depth = 0; depth < maxDepth; depth++) {
    const auto presentLevel = graph.getVertexIdsAtDepth(depth);
    const auto destinationLevel = graph.getVertexIdsAtDepth(depth + 1);
    for (const auto& fromVertexId : presentLevel) {
      if (isLucky(getProbabilityYellow(depth, maxDepth))) {
        const std::lock_guard lock(mutex);
        const auto unconnectedVertexIds =
            getUnconnectedVertexIds(fromVertexId, destinationLevel, graph);
        if (!unconnectedVertexIds.empty()) {
          const auto randomVertexId = getRandomVertexId(unconnectedVertexIds);
          graph.addEdge(fromVertexId, randomVertexId,
                        uni_course_cpp::Edge::Color::Yellow);
        }
      }
    }
  }
}

}  // namespace
namespace uni_course_cpp {
GraphGenerator::GraphGenerator(int maxDepth, int newVerticesNum)
    : maxDepth_(maxDepth), newVerticesNum_(newVerticesNum) {}

void GraphGenerator::generateVertices(Graph& graph, int firstVertexId) const {
  using JobCallback = std::function<void()>;
  auto jobs = std::list<JobCallback>();

  std::atomic<int> jobsDone = 0;
  std::mutex lockGraph;

  for (int i = 0; i < newVerticesNum_; i++) {
    jobs.emplace_back([this, &graph, &firstVertexId, &lockGraph, &jobsDone]() {
      generateGrey(graph, firstVertexId, 0, lockGraph);
      jobsDone++;
    });
  }

  std::atomic<bool> shouldTerminate = false;
  std::mutex jobsLock;

  const auto worker = [&shouldTerminate, &jobsLock, &jobs]() {
    while (true) {
      if (shouldTerminate) {
        return;
      }
      const auto jobOptional = [&jobsLock,
                                &jobs]() -> std::optional<JobCallback> {
        const std::lock_guard lock(jobsLock);
        if (!jobs.empty()) {
          const auto job = jobs.front();
          jobs.pop_front();
          return job;
        }
        return std::nullopt;
      }();
      if (jobOptional.has_value()) {
        const auto& job = jobOptional.value();
        job();
      }
    }
  };

  const auto threads_count = std::min(MAX_THREADS_COUNT, newVerticesNum_);
  auto threads = std::vector<std::thread>();
  threads.reserve(threads_count);

  for (int i = 0; i < threads_count; i++) {
    threads.emplace_back(worker);
  }

  while (jobsDone != newVerticesNum_) {
  }

  shouldTerminate = true;
  for (auto& thread : threads) {
    thread.join();
  }
}

void GraphGenerator::generateGrey(Graph& graph,
                                  int parentVertexId,
                                  int parentDepth,
                                  std::mutex& lockGraph) const {
  const auto new_vertex_id = [&graph, &lockGraph, parentVertexId]() {
    const std::lock_guard lock(lockGraph);
    const auto new_vertex_id = graph.addVertex();
    graph.addEdge(parentVertexId, new_vertex_id.id, Edge::Color::Gray);
    return new_vertex_id;
  }();

  if (parentDepth + 1 >= maxDepth_) {
    return;
  }

  for (int i = 0; i < newVerticesNum_; i++) {
    if (isLucky(getProbabilityGray(parentDepth, maxDepth_))) {
      generateGrey(graph, new_vertex_id.id, parentDepth + 1, lockGraph);
    }
  }
}
Graph GraphGenerator::generate() const {
  auto graph = Graph();
  graph.addVertex();  // add root vertex
  generateVertices(graph, 0);
  std::mutex mutex;
  std::thread greenThread(generateGreen, std::ref(graph), std::ref(mutex));
  std::thread blueThread(generateBlue, std::ref(graph), std::ref(mutex));
  std::thread redThread(generateRed, std::ref(graph), std::ref(mutex));
  std::thread yellowThread(generateYellow, std::ref(graph), std::ref(mutex));

  greenThread.join();
  blueThread.join();
  redThread.join();
  yellowThread.join();

  return graph;
}
}  // namespace uni_course_cpp
