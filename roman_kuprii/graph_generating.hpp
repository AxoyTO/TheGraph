#pragma once

namespace uni_cpp_practice {

class Graph;

namespace graph_generating {

void new_vertices_generation(Graph& work_graph,
                             const int& depth,
                             const int& new_vertices_num);

void add_blue_edges(Graph& work_graph);
void add_green_edges(Graph& work_graph);
void add_red_edges(Graph& work_graph);
void add_yellow_edges(Graph& work_graph);

void paint_edges(Graph& work_graph);

}  // namespace graph_generating

}  // namespace uni_cpp_practice
