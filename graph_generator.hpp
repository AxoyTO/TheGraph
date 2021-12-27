#pragma once
#include <mutex>
#include "config.hpp"
#include "graph.hpp"

namespace uni_course_cpp {
class GraphGenerator {
 public:
  struct Params {
    explicit Params(int _depth = 0, int _newVerticesNum = 0)
        : depth(_depth), newVerticesNum(_newVerticesNum) {}

    int const depth = 0;
    int const newVerticesNum = 0;
  };

  explicit GraphGenerator(const Params& params = Params()) : params_(params) {}

  Graph generate() const;

 private:
  void generateGrayEdges(Graph& graph) const;
  void generateGreenEdges(Graph& graph, std::mutex& mutex) const;
  void generateYellowEdges(Graph& graph, std::mutex& mutex) const;
  void generateRedEdges(Graph& graph, std::mutex& mutex) const;
  void generateGrayBranch(Graph& graph,
                          Depth current_depth,
                          const VertexId& current_vertex_id,
                          std::mutex& jobs_mutex) const;

 private:
  Params const params_ = Params();
};
}  // namespace uni_course_cpp
