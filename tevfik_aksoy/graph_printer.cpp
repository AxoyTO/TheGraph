#include "graph_printer.hpp"

namespace uni_cpp_practice {
std::string GraphPrinter::print() const {
  std::string json_string;
  json_string += "{\n\"vertices\": [\n";
  for (int i = 0; i < graph_.get_vertices().size(); i++) {
    json_string += graph_.get_vertices()[i].to_JSON();
    if (i + 1 != graph_.get_vertices().size())
      json_string += ",\n";
  }
  json_string += "\n  ],\n";

  json_string += "\"edges\": [\n";
  for (int i = 0; i < graph_.get_edges().size(); i++) {
    json_string += graph_.get_edges()[i].to_JSON();
    if (i + 1 != graph_.get_edges().size())
      json_string += ",\n";
  }
  json_string += "\n  ]\n}\n";
  return json_string;
}
}  // namespace uni_cpp_practice
