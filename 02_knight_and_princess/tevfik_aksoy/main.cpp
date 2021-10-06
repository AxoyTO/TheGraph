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

// Structure of nodes(vertices)
struct Vertex {
 public:
  VertexId id = 0;

  explicit Vertex(VertexId _id = 0) : id(_id) {}

  std::string to_JSON(const vector<int>& edge_ids) const {
    std::string str;

    str += "\t{ \"id\": " + to_string(id) + ", \"edge_ids\": [";
    for (int i = 0; i < edge_ids.size(); i++) {
      str += to_string(edge_ids[i]);
      if (i + 1 != edge_ids.size())
        str += ", ";
    }
    return str;
  }
};

// Structure of the edge including the source and destination nodes in it
struct Edge {
 public:
  EdgeId id = 0;
  VertexId source;
  VertexId destination;

  Edge(int src_id, int dest_id, int _id)
      : id(_id), source(src_id), destination(dest_id) {}

  std::string to_JSON() const {
    std::string str;
    str += "\t{ \"id\": " + to_string(id) + ", \"vertex_ids\": [" +
           to_string(source) + ", " + to_string(destination) + "] }";
    return str;
  }
};

class Graph {
 public:
  // edges carry information about their source and destination vertices
  vector<Edge> edges;
  array<Vertex, 13> vertex;
  // vertices carry information about edges that connect them with other
  // vertices
  vector<vector<dest_id_edge_id>> vertices;

  Graph() {}

  std::string to_JSON() {
    vector<int> edge_ids;
    std::string str;
    str += "{\n\"vertices\": [\n";
    for (int i = 0; i < vertices.size(); i++) {
      for (const auto& j : vertices[i]) {
        edge_ids.push_back(get<1>(j));
      }
      str += vertex[i].to_JSON(edge_ids);
      if (i + 1 == vertices.size()) {
        str += "] }\n  ],\n";
      } else {
        str += "] },\n";
      }
      edge_ids.clear();
    }

    str += "\"edges\": [\n";
    for (int i = 0; i < edges.size(); i++) {
      str += edges[i].to_JSON();
      if (i + 1 == edges.size()) {
        str += "\n";
      } else {
        str += ",\n";
      }
    }
    str += "  ]\n}\n";
    return str;
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
  // using set to exclude the repeating source nodes(the ones with more than 1
  // edge connected)
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
  auto graph = generateGraph();
  std::ofstream file("graph.json", std::ios::out);
  if (!file.is_open())
    std::cerr << "Error opening the file graph.json!" << endl;
  else {
    file << graph.to_JSON();
  }
  return 0;
}
