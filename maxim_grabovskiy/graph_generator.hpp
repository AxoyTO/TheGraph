#pragma once
#include <random>
#include "config.hpp"
#include "graph.hpp"
namespace uni_course_cpp {
class GraphGenerator {
 public:
  struct Params {
    explicit Params(int _depth = 0, int _new_vertices_num = 0)
        : depth(_depth), new_vertices_num(_new_vertices_num) {}

    int const depth = 0;
    int const new_vertices_num = 0;
  };

  explicit GraphGenerator(const Params& params = Params()) : params_(params) {}

  Graph generate() const;

 private:
  bool checkProbability(float probability) const;
  int randomIntNumber(float maximum) const;

  std::vector<VertexId> getUnconnectedVertexIds(
      VertexId vertexId,
      const std::vector<VertexId>& vertexIds,
      Graph const& graph) const;

  void generateGrayEdges(Graph& graph) const;
  void generateGreenEdges(Graph& graph) const;
  void generateYellowEdges(Graph& graph) const;
  void generateRedEdges(Graph& graph) const;

 private:
  Params const params_ = Params();
};
}  // namespace uni_course_cpp
