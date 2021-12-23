#include "Edge.hpp"
#include "Graph.hpp"

using Graph = uni_course_cpp::Graph;
using Edge = uni_course_cpp::Edge;
using Vertex = uni_course_cpp::Vertex;
namespace uni_course_cpp {
class GraphPrinter {
 public:
  static std::string printGraph(const Graph& graph);
  static std::string printVertex(const Vertex& vertex);
  static std::string printEdge(const Edge& edge);
  static std::string printEdgeColor(const Edge::Color& color);
};

}  // namespace uni_course_cpp
