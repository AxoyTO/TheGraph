#pragma once
#include "graph.hpp"
namespace uni_course_cpp {
class GraphGenerator {
 public:
  struct Params {
    explicit Params(const Depth& _depth = 0, int _new_vertices_num = 0)
        : depth(_depth), new_vertices_num(_new_vertices_num) {}

    const Depth depth = 0;
    const int new_vertices_num = 0;
  };

  explicit GraphGenerator(const Params& params = Params()) : params_(params){};
  Graph generate_graph() const;

 private:
  const Params params_ = Params();
  void generate_vertices(Graph& graph) const;
};
}  // namespace uni_course_cpp