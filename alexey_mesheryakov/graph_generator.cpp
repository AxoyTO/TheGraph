#include "graph_generator.hpp"
#include <assert.h>
#include <stdlib.h>
#include <time.h>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include "graph.hpp"

namespace {
VertexId random_vertex_generator(Graph& graph, Depth depth, Depth max_depth) {
  const int luck = rand() % max_depth;
  if (luck < max_depth - depth)
    return graph.add_vertex();
  else
    return INVALID_ID;
}

bool random_colour_edge_generator(const Graph::Edge::Color& color,
                                  Depth depth = 0,
                                  Depth max_depth = 0) {
  int luck = 0;
  switch (color) {
    case Graph::Edge::Color::Green:
      luck = rand() % 10;
      return (luck == 0);
    case Graph::Edge::Color::Red:
      luck = rand() % 3;
      return (luck == 0);
    case Graph::Edge::Color::Blue:
      luck = rand() % 4;
      return (luck == 0);
    case Graph::Edge::Color::Yellow:
      luck = rand() % max_depth;
      return (depth > luck);
    default:
      break;
  }
  return false;
}
void generate_green_edges(Graph& graph) {
  for (Depth depth = 1; depth < graph.get_depth(); depth++) {
    auto level = graph.get_vertices_at_depth(depth);
    for (int j = 0; j < level.size(); j++)
      if (random_colour_edge_generator(Graph::Edge::Color::Green)) {
        graph.add_edge(level[j], level[j], Graph::Edge::Color::Green);
      }
  }
}
void generate_blue_edges(Graph& graph) {
  for (Depth depth = 1; depth < graph.get_depth(); depth++) {
    auto level = graph.get_vertices_at_depth(depth);
    for (int j = 0; j < level.size(); j++) {
      if (j > 0 && j < level.size() - 1 &&
          random_colour_edge_generator(Graph::Edge::Color::Blue)) {
        bool randomiser = rand() % 2;
        VertexId nearest_id = randomiser ? level[j - 1] : level[j + 1];
        if (!graph.edge_exist(level[j], nearest_id))
          graph.add_edge(level[j], nearest_id, Graph::Edge::Color::Blue);
      }
    }
  }
}

void generate_red_edges(Graph& graph) {
  for (Depth depth = 1; depth < graph.get_depth() - 2; depth++) {
    auto level = graph.get_vertices_at_depth(depth);
    for (int j = 0; j < level.size(); j++) {
      if (random_colour_edge_generator(Graph::Edge::Color::Red)) {
        auto level_for_red_edge = graph.get_vertices_at_depth(depth + 2);
        if (level_for_red_edge.size() > 0)
          graph.add_edge(level[j],
                         level_for_red_edge[rand() % level_for_red_edge.size()],
                         Graph::Edge::Color::Red);
      }
    }
  }
}

void generate_yellow_edges(Graph& graph, Depth max_depth) {
  for (Depth depth = 1; depth < graph.get_depth() - 1; depth++) {
    auto level = graph.get_vertices_at_depth(depth);
    for (int j = 0; j < level.size(); j++) {
      if (random_colour_edge_generator(Graph::Edge::Color::Yellow, depth,
                                       max_depth)) {
        auto level_for_yellow_edge = graph.get_vertices_at_depth(depth + 1);
        vector<VertexId> new_vertices(0);
        for (const auto& vertex_id : level_for_yellow_edge)
          if (!graph.edge_exist(level[j], vertex_id))
            new_vertices.push_back(vertex_id);
        if (new_vertices.size() > 0)
          graph.add_edge(level[j], new_vertices[rand() % new_vertices.size()],
                         Graph::Edge::Color::Yellow);
      }
    }
  }
}
}  // namespace
void Graph_generator::generate_vertices(Graph& graph) const {
  graph.add_vertex();
  for (Depth depth = 1; depth < params_.depth; depth++) {
    auto level = graph.get_vertices_at_depth(depth - 1);
    for (const auto& vertex_id : level)
      for (int j = 0; j < params_.new_vertices_num; j++) {
        VertexId new_vertex_id =
            random_vertex_generator(graph, depth - 1, params_.depth);
        if (new_vertex_id != INVALID_ID) {
          graph.add_edge(vertex_id, new_vertex_id);
        }
      }
  }
}

Graph Graph_generator::generate_graph() const {
  srand(time(0));
  Graph graph;
  generate_vertices(graph);
  generate_green_edges(graph);
  generate_blue_edges(graph);
  generate_red_edges(graph);
  generate_yellow_edges(graph, params_.depth);
  return graph;
}
