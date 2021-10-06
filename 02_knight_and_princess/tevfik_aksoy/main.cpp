#include <fstream>
#include <iostream>
#include <set>
#include <string>
#include <tuple>
#include <vector>

using std::endl;
using std::get;
using std::to_string;
using std::vector;

using NodeId = int;
using EdgeId = int;

// Tuple of <int,int> for realization of vector of vectors of <int,int>
using dest_edge_id = std::tuple<int, int>;
void generateVertices(vector<vector<dest_edge_id>>& graphVec,
                      vector<std::pair<int, int>>& edges_src_dest);
// Structure of nodes(vertices)
struct Node {
 public:
  NodeId id = 0;
  explicit Node(NodeId _id = 0) : id(_id) {}

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
  Node src_node;
  Node dest_node;
  Edge(int src_id, int dest_id, int _id)
      : id(_id), src_node(src_id), dest_node(dest_id) {}

  std::string to_JSON() const {
    std::string str;
    str += "\t{ \"id\": " + to_string(id) + ", \"vertex_ids\": [" +
           to_string(src_node.id) + ", " + to_string(dest_node.id) + "] }";
    return str;
  }
};

class Graph {
 private:
  vector<std::pair<int, int>>
      edges_src_dest_;  // vector of pairs to keep data of source and
                        // destination vertices of an edge
  vector<vector<dest_edge_id>> graph_vec_;

 public:
  Graph() { generateVertices(graph_vec_, edges_src_dest_); }

  std::string to_JSON() const {
    Node src{};
    Edge e(0, 0, 0);
    vector<int> edge_ids;
    std::string str;
    str += "{\n\"vertices\": [\n";
    for (int i = 0; i < graph_vec_.size(); i++) {
      src.id = i;
      for (const auto& j : graph_vec_[i]) {
        edge_ids.push_back(get<1>(j));
      }
      str += src.to_JSON(edge_ids);
      if (i + 1 == graph_vec_.size()) {
        str += "] }";
      } else
        str += "] },";
      if (i + 1 == graph_vec_.size())
        str += "\n  ],\n";
      else
        str += "\n";
      edge_ids.clear();
    }

    str += "\"edges\": [\n";
    for (int i = 0; i < edges_src_dest_.size(); i++) {
      e.id = i;
      e.src_node.id = edges_src_dest_[i].first;
      e.dest_node.id = edges_src_dest_[i].second;

      str += e.to_JSON();
      if (i + 1 == edges_src_dest_.size()) {
        str += "\n";
      } else {
        str += ",\n";
      }
    }
    str += "  ]\n}\n";
    return str;
  }
};

void generateVertices(vector<vector<dest_edge_id>>& graphVec,
                      vector<std::pair<int, int>>& edges_src_dest) {
  Edge edges[] = {{0, 1, 0},    {0, 2, 1},   {0, 3, 2},   {1, 4, 3},
                  {1, 5, 4},    {1, 6, 5},   {2, 7, 6},   {2, 8, 7},
                  {3, 9, 8},    {4, 10, 9},  {5, 10, 10}, {6, 10, 11},
                  {7, 11, 12},  {8, 11, 13}, {9, 12, 14}, {10, 13, 15},
                  {11, 13, 16}, {12, 13, 17}};
  // using set to exclude the repeating source nodes(the ones with more than 1
  // edge connected)
  std::set<int> nodes_set;
  for (auto edge : edges) {
    nodes_set.insert(edge.src_node.id);
  }
  graphVec.resize(nodes_set.size() + 1);
  for (const auto& edge : edges) {
    const int src_id = edge.src_node.id;
    if (src_id >= graphVec.size())
      continue;
    const int dest_id = edge.dest_node.id;
    const int edge_id = edge.id;

    edges_src_dest.emplace_back(src_id, dest_id);

    graphVec[src_id].push_back(std::make_tuple(dest_id, edge_id));
    graphVec[dest_id].push_back(std::make_tuple(src_id, edge_id));
  }
}

int main() {
  const auto graph = Graph();
  std::ofstream file("graph.json", std::ios::out);
  if (!file.is_open())
    std::cerr << "Error opening the file graph.json!" << endl;
  else {
    file << graph.to_JSON();
  }
  return 0;
}
