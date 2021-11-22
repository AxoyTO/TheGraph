#ifndef GRAPHPRINTER_H
#define GRAPHPRINTER_H

#include <string>

class Graph;

class GraphPrinter {
 public:
  GraphPrinter(const Graph& graph) : graph_(graph) {}
  std::string print() const;

 private:
  std::string print_vertices() const;
  std::string print_edges() const;

 private:
  const Graph& graph_;
};

#endif  // GRAPHPRINTER_H
