#pragma once
#include <sstream>

namespace uni_course_cpp {
class Graph;
class Game;
class GraphPath;

namespace graph_printing {
std::string print_graph(const Graph& graph);
std::string print_graph_description(const Graph& graph);
std::string print_path(const GraphPath& path);
std::string print_game(const Game& game);
}  // namespace graph_printing
}  // namespace uni_course_cpp
