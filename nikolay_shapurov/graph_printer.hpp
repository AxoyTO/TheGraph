#pragma once

#include <string>
#include <unordered_map>
#include "graph.hpp"

class GraphPrinter {
 public:
  GraphPrinter(const Graph& graph);
  std::string print() const;

  std::string print_vertex(const Vertex& vertex) const;
  std::string print_edge(const Edge& edge) const;

 private:
  std::string print_vertices() const;
  std::string print_edges() const;

 private:
  const Graph& graph_;

  mutable std::unordered_map<VertexId, std::vector<EdgeId>> adjacency_map_;
};
