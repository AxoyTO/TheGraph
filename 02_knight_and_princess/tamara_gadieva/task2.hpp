#include <fstream>
#include <iostream>
#include <vector>

using VertexId = int;
using EdgeId = int;

struct Vertex {
  VertexId id;
  std::vector<EdgeId> edges;

  void json_string(std::ofstream& graph_json) const;
};

struct Edge {
  EdgeId id;
  VertexId vertex_1;
  VertexId vertex_2;

  void json_string(std::ofstream& graph_json) const;
};