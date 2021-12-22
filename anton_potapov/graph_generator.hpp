#pragma once

#include "graph.hpp"

namespace uni_cource_cpp {
class GraphGenerator {
 public:
  struct Params {
    explicit Params(int depth_val = 0, int new_vertices_num_val = 0);
    const int depth;
    const int new_vertices_num;
  };
  explicit GraphGenerator(const Params& params = Params());
  Graph generate_graph() const;

 private:
  const Params params_{};
};
}  // namespace uni_cource_cpp
