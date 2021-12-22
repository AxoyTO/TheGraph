#pragma once
#include <string>
#include "graph.hpp"
namespace uni_course_cpp {
class GraphPrinter {
 public:
  explicit GraphPrinter(const Graph& graph) : graph_(graph){};
  std::string print() const;

 private:
  const Graph& graph_;
};

namespace printing {
std::string print_graph(const uni_course_cpp::Graph& graph);

}  // namespace printing

}  // namespace uni_course_cpp
