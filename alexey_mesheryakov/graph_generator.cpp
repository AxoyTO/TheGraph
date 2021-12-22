#include "graph_generator.hpp"
#include <assert.h>
#include <stdlib.h>
#include <time.h>
#include <algorithm>
#include <fstream>
#include <functional>
#include <iostream>
#include <limits>
#include <random>
#include <sstream>
#include <string>
#include <vector>
#include "graph.hpp"

namespace {
using std::vector;
using uni_course_cpp::Depth;
using uni_course_cpp::Edge;
using uni_course_cpp::Graph;
using uni_course_cpp::VertexId;

float get_color_probability(const Edge::Color& color,
                            const Depth& current_depth = 0,
                            const Depth& max_depth = 1) {
  switch (color) {
    case Edge::Color::Green:
      return 0.1;
    case Edge::Color::Blue:
      return 0.25;
    case Edge::Color::Red:
      return 0.33;
    case Edge::Color::Yellow:
      return static_cast<float>(current_depth) / (max_depth - 1);
    case Edge::Color::Gray:
      return 1.0 - static_cast<float>(current_depth) / (max_depth - 1);
  }
  throw std::runtime_error("Color doesn't exist");
}

bool is_lucky(float probability) {
  assert(probability + std::numeric_limits<float>::epsilon() >= 0 &&
         probability - std::numeric_limits<float>::epsilon() <= 1.0 &&
         "given probability is incorrect");
  static std::knuth_b rand_engine{};
  std::mt19937 rng{rand_engine()};
  std::bernoulli_distribution bernoullu_distribution_var(probability);
  return bernoullu_distribution_var(rng);
}

int get_random_number(int size) {
  std::random_device rd;
  std::default_random_engine gen(rd());
  std::uniform_int_distribution<int> distrib(0, size - 1);
  return distrib(gen);
}

void generate_green_edges(Graph& graph) {
  for (Depth depth = 1; depth < graph.get_depth(); depth++) {
    for (const auto vertex_id : graph.get_vertex_ids_at_depth(depth))
      if (is_lucky(get_color_probability(Edge::Color::Green))) {
        graph.add_edge(vertex_id, vertex_id);
      }
  }
}
void generate_blue_edges(Graph& graph) {
  for (Depth depth = 1; depth < graph.get_depth(); depth++) {
    const auto level = graph.get_vertex_ids_at_depth(depth);
    for (int j = 0; j < level.size(); j++) {
      if (j < level.size() - 1 &&
          is_lucky(get_color_probability(Edge::Color::Blue))) {
        graph.add_edge(level[j], level[j + 1]);
      }
    }
  }
}

void generate_red_edges(Graph& graph) {
  for (Depth depth = 1; depth < graph.get_depth() - 2; depth++) {
    for (const auto vertex_id : graph.get_vertex_ids_at_depth(depth)) {
      if (is_lucky(get_color_probability(Edge::Color::Red))) {
        auto level_for_red_edge = graph.get_vertex_ids_at_depth(depth + 2);
        if (level_for_red_edge.size() > 0)
          graph.add_edge(
              vertex_id,
              level_for_red_edge[get_random_number(level_for_red_edge.size())]);
      }
    }
  }
}

vector<VertexId> get_unconnected_vertex_ids(
    Graph& graph,
    const VertexId& vertex_id,
    const vector<VertexId>& vertex_ids) {
  vector<VertexId> new_vertices(0);
  for (const auto& new_vertex_id : vertex_ids)
    if (!graph.edge_exist(vertex_id, new_vertex_id))
      new_vertices.push_back(new_vertex_id);
  return new_vertices;
}

void generate_yellow_edges(Graph& graph, const Depth& max_depth) {
  for (Depth depth = 1; depth < graph.get_depth() - 1; depth++) {
    const auto level = graph.get_vertex_ids_at_depth(depth);
    for (const auto vertex_id : level) {
      if (is_lucky(
              get_color_probability(Edge::Color::Yellow, depth, max_depth))) {
        auto new_vertices = get_unconnected_vertex_ids(
            graph, vertex_id, graph.get_vertex_ids_at_depth(depth + 1));
        if (new_vertices.size() > 0)
          graph.add_edge(vertex_id,
                         new_vertices[get_random_number(new_vertices.size())]);
      }
    }
  }
}
}  // namespace
namespace uni_course_cpp {
void GraphGenerator::generate_vertices(Graph& graph) const {
  graph.add_vertex();
  bool level_added = true;
  for (Depth depth = 1; depth < params_.depth; depth++) {
    if (level_added) {
      auto level = graph.get_vertex_ids_at_depth(depth - 1);
      level_added = false;
      for (const auto& vertex_id : level)
        for (int j = 0; j < params_.new_vertices_num; j++) {
          if (is_lucky(get_color_probability(Edge::Color::Gray, depth - 1,
                                             params_.depth))) {
            VertexId new_vertex_id = graph.add_vertex();
            graph.add_edge(vertex_id, new_vertex_id);
            level_added = true;
          }
        }
    }
  }
}

Graph GraphGenerator::generate_graph() const {
  srand(time(0));
  Graph graph;
  generate_vertices(graph);
  generate_green_edges(graph);
  generate_blue_edges(graph);
  generate_red_edges(graph);
  generate_yellow_edges(graph, params_.depth);
  return graph;
}
}  // namespace uni_course_cpp