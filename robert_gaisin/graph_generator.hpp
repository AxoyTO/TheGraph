#pragma once

namespace uni_cource_cpp {
class Graph;
struct Params {
  int depth = 0;
  int new_vertices_count = 0;
};
Params GraphGenerationParams(int depth, int new_vertices_count);
class GraphGenerator {
 public:
  explicit GraphGenerator(Params params) : params_(params){};
  Graph generate() const;

 private:
  Params params_;
};
}  // namespace uni_cource_cpp
