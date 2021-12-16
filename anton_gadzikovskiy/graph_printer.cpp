#include "graph_printer.hpp"
#include <array>
#include <map>
#include <stdexcept>
#include "graph.hpp"

namespace uni_cource_cpp {

std::string graph_printing::print_edge_color(const Graph::Edge::Color& color) {
  switch (color) {
    case Graph::Edge::Color::Gray:
      return "gray";
    case Graph::Edge::Color::Green:
      return "green";
    case Graph::Edge::Color::Yellow:
      return "yellow";
    case Graph::Edge::Color::Red:
      return "red";
  }
  throw std::runtime_error("Failed to determine color");
}

std::string graph_printing::print_vertex(const Graph::Vertex& vertex) {
  std::string vertex_output =
      "\"id\": " + std::to_string(vertex.id) + ", " + "\"edge_ids\": [";
  for (const auto& edge_id : vertex.get_edge_ids()) {
    vertex_output += std::to_string(edge_id);
    if (edge_id != vertex.get_edge_ids().back()) {
      vertex_output += ", ";
    }
  }
  vertex_output += "], \"depth\": " + std::to_string(vertex.depth);
  return vertex_output;
}

std::string graph_printing::print_edge(const Graph::Edge& edge) {
  return "\"id\": " + std::to_string(edge.id) + ", " + "\"vertex_ids\": [" +
         std::to_string(edge.from_vertex_id) + ", " +
         std::to_string(edge.to_vertex_id) + "]" + ", " + "\"color\": \"" +
         print_edge_color(edge.color) + "\"";
}

std::string graph_printing::print_graph(const Graph& graph) {
  std::string graph_output =
      "{\n\t\"depth\": " + std::to_string(graph.depth()) +
      ",\n\t\"vertices\": [\n\t\t";
  for (const auto& [key, vertex] : graph.get_vertices()) {
    graph_output += "{" + print_vertex(vertex) + "},\n\t\t";
  }
  graph_output = graph_output.substr(0, graph_output.size() - 4) +
                 "\n\t],\n\t\"edges\": [\n\t\t";
  for (const auto& [key, edge] : graph.get_edges()) {
    graph_output += "{" + print_edge(edge) + "},\n\t\t";
  }
  graph_output = graph_output.substr(0, graph_output.size() - 4) + "\n\t]\n}\n";
  return graph_output;
}

string graph_printing::print_graph_description(const Graph& graph) {
  string graph_description =
      "  depth: " + std::to_string(graph.depth()) +
      ",\n  vertices: {amount: " + std::to_string(graph.get_vertices_amount()) +
      ", distribution: [";

  for (const auto& vertex_ids_on_depth : graph.get_vertices_on_depth()) {
    graph_description += std::to_string(vertex_ids_on_depth.size());
    if (vertex_ids_on_depth != graph.get_vertices_on_depth().back()) {
      graph_description += ", ";
    } else {
      graph_description += "]},\n";
    }
  }

  graph_description = graph_description + "  edges: {amount: " +
                      std::to_string(graph.get_edges_amount()) +
                      ", distribution: {";
  const std::array<uni_cource_cpp::Graph::Edge::Color, 4> colors = {
      uni_cource_cpp::Graph::Edge::Color::Gray,
      uni_cource_cpp::Graph::Edge::Color::Green,
      uni_cource_cpp::Graph::Edge::Color::Yellow,
      uni_cource_cpp::Graph::Edge::Color::Red};

  for (const auto& color : colors) {
    graph_description =
        graph_description + print_edge_color(color) + ": " +
        std::to_string(graph.get_colored_edge_ids(color).size()) + ", ";
  }
  graph_description =
      graph_description.substr(0, graph_description.size() - 2) + "}}";
  return graph_description;
}

}  // namespace uni_cource_cpp
