#include "graph_printing.hpp"
#include <string>
#include "graph.hpp"

namespace uni_course_cpp {
namespace printing {

std::string print_edge_color(const Graph::Edge::Color& color) {
  switch (color) {
    case Graph::Edge::Color::Grey:
      return "grey";
    case Graph::Edge::Color::Green:
      return "green";
    case Graph::Edge::Color::Yellow:
      return "yellow";
    case Graph::Edge::Color::Red:
      return "red";
    default:
      throw std::runtime_error("No such color");
  }
}

std::string print_graph(const Graph& graph) {
  std::stringstream result_stream;
  result_stream << "{" << std::endl;
  result_stream << "\tdepth: " << graph.depth() << "," << std::endl;

  result_stream << "\tvertices: {amount: " << graph.vertices_id_counter()
                << ", ";
  result_stream << "distribution: [";
  for (Graph::Depth depth = 0; depth <= graph.depth(); depth++) {
    result_stream << graph.get_vertex_ids_on_depth(depth).size();
    if (depth != graph.depth()) {
      result_stream << ", ";
    }
  }
  result_stream << "]}," << std::endl;

  result_stream << "\tedges: {amount: " << graph.edges_id_counter() << ", ";
  result_stream << "distribution: {" << graph.get_edges_distribution() << "}}";

  return result_stream.str();
}
}  // namespace printing
}  // namespace uni_course_cpp
