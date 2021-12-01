#pragma once

namespace uni_cpp_practice {

class Graph;

class GraphGenerator {
 public:
  struct Params {
    Params(int _depth, int _new_vertices_num)
        : depth(_depth), new_vertices_num(_new_vertices_num){};

    int depth = 0;
    int new_vertices_num = 0;
  };

  Graph generate();

  GraphGenerator(const Params& params) : params_(params) {}

 private:
  Params params_;
};

}  // namespace uni_cpp_practice
