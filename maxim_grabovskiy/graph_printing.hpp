
#include <string>
#include "graph.hpp"
namespace uni_course_cpp {
class GraphPrinter {
 public:
  static std::string printGraph(const uni_course_cpp::Graph& graph);
  static std::string printEdgeColor(Graph::Edge::Color const& color);
};
}  // namespace uni_course_cpp
