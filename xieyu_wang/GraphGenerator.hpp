#pragma once
#include <mutex>
#include "Graph.hpp"
namespace uni_course_cpp {
class GraphGenerator {
 public:
  struct Params {
    explicit Params(int maxDepth = 0, int newVerticesNum = 0);
    const int maxDepth;
    const int newVerticesNum;
  };
  GraphGenerator(Params params);

  Graph generate() const;

 private:
  const Params params_;
  void generateGrey(Graph& graph,
                    int parent_vertex_id,
                    int parent_depth,
                    std::mutex& lock_graph) const;

  void generateVertices(Graph& graph, int firstVertexId) const;
};

}  // namespace uni_course_cpp
