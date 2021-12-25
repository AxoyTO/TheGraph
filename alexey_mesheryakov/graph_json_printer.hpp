#pragma once
#include <string>
#include "graph.hpp"
namespace uni_course_cpp {
class GraphJsonPrinter {
 public:
  explicit GraphJsonPrinter(const Graph& graph) : graph_(graph){};
  std::string print() const;

 private:
  const Graph& graph_;
};

}  // namespace uni_course_cpp
