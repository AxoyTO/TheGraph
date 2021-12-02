#include "graph_generator.hpp"
#include <iostream>
#include <random>

using VertexId = uni_cpp_practice::VertexId;
using Graph = uni_cpp_practice::Graph;

namespace {
constexpr float GREEN_EDGE_PROBABILITY = 0.1;
constexpr float BLUE_EDGE_PROBABILITY = 0.25;
constexpr float RED_EDGE_PROBABILITY = 0.33;
float get_random_probability() {
  std::random_device rd;
  std::mt19937 mt(rd());
  std::uniform_real_distribution<float> probability(0.0, 1);
  return probability(mt);
}

VertexId get_random_vertex_id(const std::vector<VertexId>& vertices) {
  std::random_device rd;
  std::mt19937 mt(rd());
  std::uniform_int_distribution<int> random_vertex_distribution(
      0, vertices.size() - 1);
  return vertices[random_vertex_distribution(mt)];
}

std::vector<VertexId> filter_connected_vertices(
    const VertexId& id,
    const std::vector<VertexId>& vertex_ids,
    const Graph& graph) {
  std::vector<VertexId> result;
  for (const auto& vertex_id : vertex_ids) {
    if (!graph.are_vertices_connected(id, vertex_id)) {
      result.push_back(vertex_id);
    }
  }
  return result;
}
}  // namespace

namespace uni_cpp_practice {
void GraphGenerator::generate_vertices_and_gray_edges(Graph& graph) const {
  graph.insert_vertex();
  for (VertexDepth depth = 0; depth < params_.max_depth; depth++) {
    bool is_new_vertex_generated = false;
    const float probability = (float)depth / (float)params_.max_depth;
    for (const auto& source : graph.get_vertices_in_depth(depth)) {
      for (int j = 0; j < params_.new_vertices_num; j++) {
        if (get_random_probability() > probability) {
          is_new_vertex_generated = true;
          const VertexId new_vertex = graph.insert_vertex();
          graph.insert_edge(source, new_vertex);
        }
      }
    }
    if (!is_new_vertex_generated)
      break;
  }

  if (params_.max_depth != graph.depth()) {
    std::cout << "Max depth couldn't be reached. Depth of final vertex: "
              << graph.depth() << "\n";
  }
}

void GraphGenerator::generate_green_edges(Graph& graph) const {
  for (const auto& vertex : graph.get_vertices()) {
    if (get_random_probability() < GREEN_EDGE_PROBABILITY) {
      graph.insert_edge(vertex.id, vertex.id);
    }
  }
}

void GraphGenerator::generate_blue_edges(Graph& graph) const {
  for (int depth = 0; depth < graph.depth(); depth++) {
    const auto& vertices_in_depth = graph.get_vertices_in_depth(depth);
    for (VertexId j = 0; j < vertices_in_depth.size() - 1; j++) {
      const auto source = vertices_in_depth[j];
      const auto destination = vertices_in_depth[j + 1];
      if (get_random_probability() < BLUE_EDGE_PROBABILITY) {
        graph.insert_edge(source, destination);
      }
    }
  }
}

void GraphGenerator::generate_yellow_edges(Graph& graph) const {
  for (VertexDepth depth = 0; depth < graph.depth(); depth++) {
    float probability = 1 - (float)depth * (1 / (float)(graph.depth() - 1));
    const auto& vertices = graph.get_vertices_in_depth(depth);
    const auto& vertices_next = graph.get_vertices_in_depth(depth + 1);
    for (const auto& vertex_id : vertices) {
      if (get_random_probability() > probability) {
        const auto& filtered_vertex_ids =
            filter_connected_vertices(vertex_id, vertices_next, graph);
        if (filtered_vertex_ids.size() == 0) {
          continue;
        }
        VertexId random_vertex_id = get_random_vertex_id(filtered_vertex_ids);
        graph.insert_edge(vertex_id, random_vertex_id);
      }
    }
  }
}

void GraphGenerator::generate_red_edges(Graph& graph) const {
  for (VertexDepth depth = 0; depth < graph.depth() - 1; depth++) {
    const auto& vertices = graph.get_vertices_in_depth(depth);
    const auto& vertices_next = graph.get_vertices_in_depth(depth + 2);
    for (const auto& vertex : vertices) {
      if (get_random_probability() < RED_EDGE_PROBABILITY) {
        graph.insert_edge(vertex, get_random_vertex_id(vertices_next));
      }
    }
  }
}

Graph GraphGenerator::generate() const {
  Graph graph;
  generate_vertices_and_gray_edges(graph);
  generate_green_edges(graph);
  generate_blue_edges(graph);
  generate_yellow_edges(graph);
  generate_red_edges(graph);

  return graph;
}
}  // namespace uni_cpp_practice
