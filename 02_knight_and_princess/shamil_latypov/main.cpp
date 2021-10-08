#include <stdlib.h>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

enum { INVALID_ID = -1 };

using Vertex_id = int;
using Edge_id = int;

class Vertex {
 public:
  Vertex_id id = INVALID_ID;
  std::vector<Edge_id> edge_ids;

  Vertex() {}
  Vertex(Vertex_id id_) : id(id_) {}

  void to_string(std::ofstream& out_json) const {
    out_json << "{ \"id\": " << id << ", \"edge_ids\": [";
    bool check = true;

    for (const auto& edge_id : edge_ids) {
      if (check) {
        check = false;
      } else {
        out_json << ", ";
      }
      out_json << edge_id;
    }
    out_json << "] }";
  }
};

class Edge {
 public:
  Edge_id id = INVALID_ID;
  Vertex_id v1;
  Vertex_id v2;

  Edge() {}
  Edge(Edge_id id_, Vertex_id v1_, Vertex_id v2_) : id(id_), v1(v1_), v2(v2_) {}

  void to_string(std::ofstream& out_json) const {
    out_json << "{ \"id\": " << id << ", \"vertex_ids\": [" << v1 << ", " << v2
             << "] }";
  }
};

void output_graph(const std::vector<Edge>& edge_mas,
                  const std::vector<Vertex>& vert_mas,
                  int numedge,
                  int numvert) {
  std::ofstream out_json("out.json");
  out_json << "{\n  \"vertices\": [\n    ";
  bool check_first_comma = false;

  for (int i = 0; i < numvert; i++) {
    if (check_first_comma) {
      out_json << ",\n    ";
    }
    check_first_comma = true;

    vert_mas[i].to_string(out_json);
  }

  out_json << "\n  ],\n  \"edges\": [\n    ";
  check_first_comma = false;

  for (int i = 0; i < numedge; i++) {
    if (check_first_comma) {
      out_json << ",\n    ";
    }
    check_first_comma = true;

    edge_mas[i].to_string(out_json);
  }
  out_json << "\n  ]\n}\n";
  out_json.close();
}

int main() {
  const int numedge = 18, numvertex = 14;
  std::vector<Vertex> vert_mas;
  vert_mas.resize(numvertex);
  std::vector<Edge> edge_mas;

  for (int i = 0; i < numvertex; i++) {
    vert_mas[i].id = i;
  }

  std::vector<std::pair<int, int>> graph_data{
      {0, 1},  {0, 2},  {0, 3},  {1, 4},   {1, 5},   {1, 6},
      {2, 7},  {2, 8},  {3, 9},  {4, 10},  {5, 10},  {6, 10},
      {7, 11}, {8, 11}, {9, 12}, {10, 13}, {11, 13}, {12, 13}};

  for (int i = 0; i < numedge; i++) {
    vert_mas[graph_data[i].first].edge_ids.push_back(i);
    vert_mas[graph_data[i].second].edge_ids.push_back(i);

    edge_mas.push_back(Edge(i, graph_data[i].first, graph_data[i].second));
  }

  output_graph(edge_mas, vert_mas, numedge, numvertex);

  return 0;
}
