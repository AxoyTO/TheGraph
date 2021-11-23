#include "graph_printer.hpp"

#include <fstream>
#include <iostream>

// ВЫВОД В ФАЙЛ JSON
void write_graph_json_file(const Graph& graph) {
  auto graph_string = Graph_printer();
  graph_string.add_graph_string(graph);

  std::ofstream out("graph.json");
  out << graph_string.get_graph_string();
  out.close();
}
