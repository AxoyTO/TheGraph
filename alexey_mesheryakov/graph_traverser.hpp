#pragma once
#include <vector>
#include "graph.hpp"
#include "graph_path.hpp"

namespace uni_course_cpp {

class GraphTraverser {
 public:
  GraphTraverser(const Graph& graph) : graph_(graph) {}

  std::vector<GraphPath> find_all_paths() const;

 private:
  const Graph& graph_;
};
}  // namespace uni_course_cpp
