#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <random>
#include <string>
#include <utility>
#include <vector>
#include "graph.hpp"

#pragma once
namespace uni_course_cpp {
constexpr float GREEN_PROBABILITY = 0.1, BLUE_PROBABILITY = 0.25,
                RED_PROBABILITY = 0.33;

class GraphGenerator {
 public:
  struct Params {
    explicit Params(int _depth = 0, int _new_vertexes_num = 0)
        : depth(_depth), new_vertexes_num(_new_vertexes_num) {}

    const int depth;
    const int new_vertexes_num;
  };

  explicit GraphGenerator(const Params& params = Params()) : params_(params) {}

  Graph generateMainBody() const;
  void generateColorEdges(Graph& graph) const;
  Graph generate() const;

 private:
  const Params params_ = Params();
  bool randomValue(float probability) const;
  int getRandomVertexId(const std::vector<VertexId> vertex_ids) const;
  float getGreyProbability(float step, int depth) const {
    return 1.0 - step * depth;
  }
  float getYellowProbability(const Graph& graph,
                             const VertexId& vertex_id) const {
    return 1.0 * graph.vertexDepth(vertex_id) / (graph.depth() - 1);
  }
};
}  // namespace uni_course_cpp
