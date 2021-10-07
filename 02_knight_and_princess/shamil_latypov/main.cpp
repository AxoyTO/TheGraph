#include <stdlib.h>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

std::ofstream out_json("out.json");

struct Vertex {
  int id = 0;

  std::vector<int> edge_ids;

  void to_string() {
    out_json << "{ \"id\": " << id << ", \"edge_ids\": [";
    bool check = true;
    for (auto i = edge_ids.begin(); i != edge_ids.end(); i++) {
      if (check)
        check = false;
      else
        out_json << ", ";
      out_json << *i;
    }
    out_json << "] }";
  }
};

struct Edge {
  int id = 0;
  struct Vertex* v1;
  struct Vertex* v2;

  void to_string() {
    out_json << "{ \"id\": " << id << ", \"vertex_ids\": [" << v1->id << ", "
             << v2->id << "] }";
  }
};

void get_graph(Edge* edge_mas, Vertex* vert_mas, int numedge, int numvertex) {
  std::pair<int, int> graph_data[numedge] = {
      {0, 1},  {0, 2},  {0, 3},  {1, 4},   {1, 5},   {1, 6},
      {2, 7},  {2, 8},  {3, 9},  {4, 10},  {5, 10},  {6, 10},
      {7, 11}, {8, 11}, {9, 12}, {10, 13}, {11, 13}, {12, 13}};

  for (int i = 0; i < numedge; i++) {
    vert_mas[graph_data[i].first].edge_ids.push_back(i);
    vert_mas[graph_data[i].second].edge_ids.push_back(i);

    edge_mas[i].v1 = &vert_mas[graph_data[i].first];
    edge_mas[i].v2 = &vert_mas[graph_data[i].second];
  }
}

void output_graph(Edge* edge_mas, Vertex* vert_mas, int numedge, int numvert) {
  out_json << "{\n  \"vertices\": [\n    ";
  bool check_first_comma = false;

  for (int i = 0; i < numvert; i++) {
    if (check_first_comma)
      out_json << ",\n    ";
    check_first_comma = true;

    vert_mas[i].to_string();
  }

  out_json << "\n  ],\n  \"edges\": [\n    ";
  check_first_comma = false;

  for (int i = 0; i < numedge; i++) {
    if (check_first_comma)
      out_json << ",\n    ";
    check_first_comma = true;

    edge_mas[i].to_string();
  }
  out_json << "\n  ]\n}\n";
}

int main() {
  int numedge = 18, numvertex = 14;
  Vertex vert_mas[numvertex];
  Edge edge_mas[numedge];

  for (int i = 0; i < numedge; i++)
    edge_mas[i].id = i;
  for (int i = 0; i < numvertex; i++)
    vert_mas[i].id = i;

  get_graph(edge_mas, vert_mas, numedge, numvertex);

  output_graph(edge_mas, vert_mas, numedge, numvertex);

  out_json.close();
  return 0;
}
