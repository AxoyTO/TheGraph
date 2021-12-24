#include "graph_printing.hpp"
#include <fstream>
#include <sstream>
#include <string>
#include "graph.hpp"

namespace uni_course_cpp {
namespace printing {
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
