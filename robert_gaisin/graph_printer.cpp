#include "graph_printer.hpp"
#include "game.hpp"
#include "graph_path.hpp"

#include <iostream>

namespace {
using uni_course_cpp::Edge;
using uni_course_cpp::EdgeColor;
using uni_course_cpp::Game;
using uni_course_cpp::Graph;
using uni_course_cpp::GraphPath;
using uni_course_cpp::Vertex;

using std::cin;
using std::cout;
using std::endl;
using std::vector;

std::string color_to_string(const EdgeColor& color) {
  switch (color) {
    case EdgeColor::Gray:
      return "\"gray\"";
    case EdgeColor::Green:
      return "\"green\"";
    case EdgeColor::Blue:
      return "\"blue\"";
    case EdgeColor::Yellow:
      return "\"yellow\"";
    case EdgeColor::Red:
      return "\"red\"";
  }
  throw std::runtime_error("Color output error");
}

std::ostream& operator<<(std::ostream& out, const vector<int>& int_vector) {
  for (auto it = int_vector.begin(); it != int_vector.end(); ++it) {
    out << *it;
    if (it != int_vector.end() - 1) {
      out << ", ";
    }
  }
  return out;
}

std::ostream& operator<<(std::ostream& out, const Vertex& vertex) {
  out << "{" << endl;
  out << "      \"id\": " << vertex.id << "," << endl;
  out << "      \"edge_ids\": [" << vertex.edge_ids() << "]," << endl;
  out << "      \"depth\": " << vertex.depth << endl;
  out << "}";
  return out;
}

std::ostream& operator<<(std::ostream& out, const Edge& edge) {
  out << "{" << endl;
  out << "      \"id\": " << edge.id << "," << endl;
  out << "      \"vertex_ids\": [" << edge.begin << ", " << edge.end << "],"
      << endl;
  out << "      \"color\": ";
  out << color_to_string(edge.color) << ",";
  out << endl << "      \"duration\": " << edge.duration() << endl;
  out << "}";
  return out;
}

std::ostream& operator<<(std::ostream& out, const vector<Vertex>& layer) {
  for (auto it = layer.begin(); it != layer.end(); ++it) {
    out << *it;
    if (it != layer.end() - 1) {
      out << ", ";
    }
  }
  return out;
}

std::ostream& operator<<(std::ostream& out, const vector<Edge>& edges) {
  for (auto it = edges.begin(); it != edges.end(); ++it) {
    out << *it;
    if (it != edges.end() - 1) {
      out << ", ";
    }
  }
  return out;
}

std::ostream& operator<<(std::ostream& out, const Graph& graph) {
  out << "{" << endl;
  out << "  \"depth\": " << graph.depth() << "," << endl;
  out << "  \"vertices\": [" << endl
      << "   " << graph.vertices() << endl
      << "  ]," << endl;
  out << "  \"edges\": [" << endl
      << "    " << graph.edges() << endl
      << "  ]" << endl
      << "}" << endl;
  return out;
}

std::string print_position(const Vertex& vertex) {
  std::ostringstream out;
  out << "{vertex_id: " << vertex.id << ", depth: " << vertex.depth << "}";
  return out.str();
}

}  // namespace
namespace uni_course_cpp {
namespace graph_printing {
std::string print_graph(const Graph& graph) {
  std::ostringstream out_message;
  out_message << graph;
  return out_message.str();
}

std::string print_graph_description(const Graph& graph) {
  std::stringstream log_string;

  const int depth = graph.depth();

  log_string << "{\n"
             << "  depth: " << depth << ",\n";

  log_string << "  vertices: {amount: " << graph.vertices().size()
             << ", distribution: [";
  for (auto current_depth = 0; current_depth <= depth; ++current_depth) {
    log_string << graph.depth_map()[current_depth].size();
    if (current_depth != depth)
      log_string << ", ";
  }
  log_string << "]},\n";

  log_string << "  edges: {amount: " << graph.edges().size()
             << ", distribution: {";

  const std::vector<EdgeColor> colors_of_edges = {
      EdgeColor::Gray, EdgeColor::Green, EdgeColor::Yellow, EdgeColor::Red,
      EdgeColor::Blue};

  for (const auto& color : colors_of_edges) {
    log_string << color_to_string(color) << ": "
               << graph.get_colored_edges(color).size();
    if (color != colors_of_edges.back())
      log_string << ", ";
  }

  log_string << "}}\n}\n";

  return log_string.str();
}

std::string print_path(const GraphPath& path) {
  std::stringstream res;

  res << "{vertices: [";

  const auto& vertex_ids = path.get_vertex_ids();

  res << vertex_ids;

  res << "], distance: ";
  res << path.distance() << ", duration: " << path.duration();
  res << "}";

  return res.str();
}
std::string print_game(const Game& game) {
  std::ostringstream out;
  const auto& graph = game.map();
  const auto& knight_vertex = graph.get_vertex(game.knight_position());
  const auto& princess_vertex = graph.get_vertex(game.princess_position());
  out << "{\n"
      << "  map: " << print_graph_description(graph) << ",\n"
      << "  knight_position: " << print_position(knight_vertex) << ",\n"
      << "  princess position: " << print_position(princess_vertex) << "\n"
      << "}";
  return out.str();
}
}  // namespace graph_printing

}  // namespace uni_course_cpp
