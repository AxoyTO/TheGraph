#pragma once

#ifndef _GRAPH_GENERATOR_
#define _GRAPH_GENERATOR_

#include "graph.hpp"

constexpr float GREEN_EDGE_PROBABILITY = 0.1;
constexpr float BLUE_EDGE_PROBABILITY = 0.25;
constexpr float RED_EDGE_PROBABILITY = 0.33;

float get_random_probability();
graph_structures::VertexId get_random_vertex_id(
    const std::vector<graph_structures::VertexId>& vertices);
void generate_vertices_and_gray_edges(
    Graph& graph,
    const graph_structures::VertexDepth& max_depth,
    const int new_vertices_num);

void generate_green_edges(Graph& graph);
void generate_blue_edges(Graph& graph);

std::vector<graph_structures::VertexId> filter_connected_vertices(
    const graph_structures::VertexId& id,
    const std::vector<graph_structures::VertexId>& vertex_ids,
    const Graph& graph);
void generate_yellow_edges(Graph& graph);

void generate_red_edges(Graph& graph);

const Graph generate_graph(const graph_structures::VertexDepth& max_depth,
                           const int new_vertices_num);

#endif
