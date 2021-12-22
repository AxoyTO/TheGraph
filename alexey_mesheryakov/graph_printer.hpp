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
}  // namespace uni_course_cpp