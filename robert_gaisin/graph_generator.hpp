#pragma once

namespace uni_cource_cpp {
class Graph;
struct Params {
  Params(int depth, int new_vertices_count)
      : depth(depth), new_vertices_count(new_vertices_count){};
  int depth = 0;
  int new_vertices_count = 0;
};

class GraphGenerator {
 public:
  explicit GraphGenerator(const Params& params) : params_(params){};
  Graph generate() const;

 private:
  Params params_;
};
}  // namespace uni_cource_cpp
