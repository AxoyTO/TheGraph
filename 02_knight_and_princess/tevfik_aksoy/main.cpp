#include <array>
#include <fstream>
#include <iostream>
#include <set>
#include <string>
#include <tuple>
#include <vector>

using std::array;
using std::endl;
using std::get;
using std::to_string;
using std::vector;

using VertexId = int;
using EdgeId = int;

// Tuple of <int,int> for realization of vector of vectors of <int,int>
using dest_id_edge_id = std::tuple<int, int>;

struct Vertex {
 public:
  VertexId id = 0;

  explicit Vertex(VertexId _id = 0) : id(_id) {}

  std::string to_JSON(const vector<int>& edge_ids) const {
    std::string json_string;

    json_string += "\t{ \"id\": " + to_string(id) + ", \"edge_ids\": [";
    for (int i = 0; i < edge_ids.size(); i++) {
      json_string += to_string(edge_ids[i]);
      if (i + 1 != edge_ids.size())
        json_string += ", ";
    }
    return json_string;
  }
};

struct Edge {
 public:
  EdgeId id = 0;
  VertexId source;
  VertexId destination;

  Edge(int src_id, int dest_id, int _id)
      : id(_id), source(src_id), destination(dest_id) {}

  std::string to_JSON() const {
    std::string json_string;
    json_string += "\t{ \"id\": " + to_string(id) + ", \"vertex_ids\": [" +
                   to_string(source) + ", " + to_string(destination) + "] }";
    return json_string;
  }
};

class Graph {
 public:
  vector<Edge> edges;
  array<Vertex, 13> vertex;
  vector<vector<dest_id_edge_id>> vertices;

  Graph() {}

  std::string to_JSON() const {
    vector<int> edge_ids;
    std::string json_string;
    json_string += "{\n\"vertices\": [\n";
    for (int i = 0; i < vertices.size(); i++) {
      for (const auto& j : vertices[i]) {
        edge_ids.push_back(get<1>(j));
      }
      json_string += vertex[i].to_JSON(edge_ids);
      if (i + 1 == vertices.size()) {
        json_string += "] }\n  ],\n";
      } else {
        json_string += "] },\n";
      }
      edge_ids.clear();
    }

    json_string += "\"edges\": [\n";
    for (int i = 0; i < edges.size(); i++) {
      json_string += edges[i].to_JSON();
      if (i + 1 == edges.size()) {
        json_string += "\n";
      } else {
        json_string += ",\n";
      }
    }
    json_string += "  ]\n}\n";
    return json_string;
  }
};

const Graph generateGraph() {
  Graph graph;
  const array<Edge, 18> edges = {{{0, 1, 0},
                                  {0, 2, 1},
                                  {0, 3, 2},
                                  {1, 4, 3},
                                  {1, 5, 4},
                                  {1, 6, 5},
                                  {2, 7, 6},
                                  {2, 8, 7},
                                  {3, 9, 8},
                                  {4, 10, 9},
                                  {5, 10, 10},
                                  {6, 10, 11},
                                  {7, 11, 12},
                                  {8, 11, 13},
                                  {9, 12, 14},
                                  {10, 13, 15},
                                  {11, 13, 16},
                                  {12, 13, 17}}};
  std::set<int> nodes_set;
  for (const auto& e : edges) {
    nodes_set.insert(e.source);
  }
  for (int i = 0; i < nodes_set.size() + 1; i++)
    graph.vertex[i].id = i;

  graph.vertices.resize(nodes_set.size() + 1);
  for (const auto& edge : edges) {
    const int src_id = edge.source;
    if (src_id >= graph.vertices.size())
      continue;
    const int dest_id = edge.destination;
    const int edge_id = edge.id;

    graph.edges.emplace_back(src_id, dest_id, edge_id);
    graph.vertices[src_id].push_back(std::make_tuple(dest_id, edge_id));
    graph.vertices[dest_id].push_back(std::make_tuple(src_id, edge_id));
  }
  return graph;
}

int main() {
  const auto graph = generateGraph();
  std::ofstream file("graph.json", std::ios::out);
  if (!file.is_open())
    std::cerr << "Error opening the file graph.json!" << endl;
  else {
    file << graph.to_JSON();
  }
  return 0;
}
