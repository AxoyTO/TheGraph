#pragma once

#include <mutex>
#include "Graph.hpp"

namespace uni_course_cpp {
class GraphGenerator {
 public:
  GraphGenerator(int maxDepth, int newVerticesNum);

  Graph generate() const;

 private:
  int maxDepth_;
  int newVerticesNum_;

  void generateGrey(Graph& graph,
                    const int parent_vertex_id,
                    int parent_depth,
                    std::mutex& lock_graph) const;

  void generateVertices(Graph& graph, const int& first_vertex_id) const;

  float getProbabilityGray(int depth) const;
};

int getRandomVertexId(const std::vector<int> vector1);

bool isLucky(float probability);

}  // namespace uni_course_cpp
