#include <stdlib.h>
#include <algorithm>
#include <array>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

std::ofstream out_json("out.json");

class Vertex {
 public:
  int id = 0;
  std::vector<int> edge_ids;

  void to_string() {
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
  int id = 0;
  Vertex* v1;
  Vertex* v2;

  Edge(int id_, Vertex* v1_, Vertex* v2_) : id(id_), v1(v1_), v2(v2_) {}

  void to_string() {
    out_json << "{ \"id\": " << id << ", \"vertex_ids\": [" << v1->id << ", "
             << v2->id << "] }";
  }
};

template <size_t SIZE>
void output_graph(std::vector<Edge> edge_mas,
                  std::array<Vertex, SIZE> vert_mas,
                  int numedge,
                  int numvert) {
  out_json << "{\n  \"vertices\": [\n    ";
  bool check_first_comma = false;

  for (int i = 0; i < numvert; i++) {
    if (check_first_comma) {
      out_json << ",\n    ";
    }
    check_first_comma = true;

    vert_mas[i].to_string();
  }

  out_json << "\n  ],\n  \"edges\": [\n    ";
  check_first_comma = false;

  for (int i = 0; i < numedge; i++) {
    if (check_first_comma) {
      out_json << ",\n    ";
    }
    check_first_comma = true;

    edge_mas[i].to_string();
  }
  out_json << "\n  ]\n}\n";
}

int main() {
  const int numedge = 18, numvertex = 14;
  std::array<Vertex, numvertex> vert_mas;
  std::vector<Edge> edge_mas;
  
  for (int i = 0; i < numvertex; i++) {
    vert_mas[i].id = i;
  }

  std::pair<int, int> graph_data[numedge] = {
      {0, 1},  {0, 2},  {0, 3},  {1, 4},   {1, 5},   {1, 6},
      {2, 7},  {2, 8},  {3, 9},  {4, 10},  {5, 10},  {6, 10},
      {7, 11}, {8, 11}, {9, 12}, {10, 13}, {11, 13}, {12, 13}};

  for (int i = 0; i < numedge; i++) {
    vert_mas[graph_data[i].first].edge_ids.push_back(i);
    vert_mas[graph_data[i].second].edge_ids.push_back(i);

    edge_mas.push_back((Edge){i, &vert_mas[graph_data[i].first],
                              &vert_mas[graph_data[i].second]});
  }

  output_graph(edge_mas, vert_mas, numedge, numvertex);

  out_json.close();
  return 0;
}
