#pragma once

#include "graph.hpp"

namespace uni_cpp_practice {
namespace graph_generation {

double get_color_probability(const Edge::Color& color);

bool is_lucky(double probability);
int get_random_number(int size);

void generate_green_edges(Graph& graph);

void generate_blue_edges(Graph& graph);

void generate_yellow_edges(Graph& graph);

void generate_red_edges(Graph& graph);

void generate_gray_edges(Graph& graph,
                         const Depth& depth,
                         const int new_vertices_num);

Graph generate_graph(const Depth& depth, int new_vertices_num);
}  // namespace graph_generation
}  // namespace uni_cpp_practice
