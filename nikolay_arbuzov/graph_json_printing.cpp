#include "graph_json_printing.hpp"
#include <sstream>
#include <string>
#include "graph.hpp"
#include "graph_printing.hpp"

namespace uni_course_cpp {
namespace printing {
namespace json {
std::string print_vertex(const Graph& graph, const Graph::Vertex& vertex) {
  std::stringstream result_stream;
  result_stream << "    {" << std::endl;
  result_stream << "      \"id\": " << vertex.id << "," << std::endl;
  result_stream << "      \"edge_ids\": [";

  const auto& edge_ids = graph.connected_edge_ids(vertex.id);

  for (const auto& edge_id : edge_ids) {
    result_stream << edge_id;
    if (edge_id != *(edge_ids.rbegin())) {
      result_stream << ", ";
    }
  }

  result_stream << "]," << std::endl;
  result_stream << "      \"depth\": " << graph.get_vertex_depth(vertex.id)
                << std::endl;
  result_stream << "    }";

  return result_stream.str();
}

std::string print_edge(const Graph::Edge& edge) {
  std::stringstream result_stream;
  result_stream << "    {" << std::endl;
  result_stream << "      \"id\": " << edge.id << "," << std::endl;
  result_stream << "      \"vertex_ids\": [";
  result_stream << edge.from_vertex_id << ", " << edge.to_vertex_id;
  result_stream << "]," << std::endl;
  result_stream << "      \"color\": \"" << print_edge_color(edge.color) << "\""
                << std::endl;
  result_stream << "    }";

  return result_stream.str();
}

std::string print_graph(const Graph& graph) {
  std::stringstream result_stream;
  result_stream << "{" << std::endl;
  result_stream << "  \"depth\": " << graph.depth() << "," << std::endl;

  result_stream << "  \"vertices\": [" << std::endl;

  const auto& vertices = graph.vertices();
  for (const auto& vertex : vertices) {
    result_stream << print_vertex(graph, vertex);
    if (vertex.id != vertices.back().id) {
      result_stream << ",";
    }
    result_stream << std::endl;
  }

  result_stream << std::endl
                << "  ]"
                << "," << std::endl;

  const auto& edges = graph.edges();
  result_stream << "  \"edges\": [" << std::endl;
  for (const auto& edge : edges) {
    result_stream << print_edge(edge);
    if (edge.id != edges.back().id) {
      result_stream << ",";
    }
    result_stream << std::endl;
  }
  result_stream << "  ]" << std::endl << "}";
  return result_stream.str();
}
}  // namespace json
}  // namespace printing
}  // namespace uni_course_cpp
