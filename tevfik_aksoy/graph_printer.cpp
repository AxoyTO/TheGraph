#include "graph_printer.hpp"

std::string GraphPrinter::print() const {
  std::string json_string;
  json_string += "{\n\"vertices\": [\n";
  for (int i = 0; i < graph.get_vertices().size(); i++) {
    json_string += graph.get_vertices()[i].to_JSON();
    if (i + 1 != graph.get_vertices().size())
      json_string += ",\n";
  }
  json_string += "\n  ],\n";

  json_string += "\"edges\": [\n";
  for (int i = 0; i < graph.get_edges().size(); i++) {
    json_string += graph.get_edges()[i].to_JSON();
    if (i + 1 != graph.get_edges().size())
      json_string += ",\n";
  }
  json_string += "\n  ]\n}\n";
  return json_string;
}
