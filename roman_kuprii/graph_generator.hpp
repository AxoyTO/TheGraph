#pragma once

constexpr double GREEN_TRASHOULD = 0.1;
constexpr double BLUE_TRASHOULD = 0.25;
constexpr double RED_TRASHOULD = 0.33;

class Graph;

double get_random_number();

void new_vertices_generation(Graph& work_graph,
                              const int& depth,
                              const int& new_vertices_num);

void add_blue_edges(Graph& work_graph);
void add_green_edges(Graph& work_graph);
void add_red_edges(Graph& work_graph);
void add_yellow_edges(Graph& work_graph);

void paint_edges(Graph& work_graph);
