#include <assert.h>
#include <stdlib.h>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

enum class Numbers { INVALID_ID = -1 };

using Vertex_id = int;
using Edge_id = int;

class Vertex {
 public:
  Vertex_id id;

  std::vector<Edge_id> edge_ids;

  Vertex() { id = (Vertex_id)Numbers::INVALID_ID; }
  Vertex(Vertex_id id_) : id(id_) {}

  void to_string(std::ofstream& out_json) const {
    out_json << "{\n      \"id\": " << id << ",\n      \"edge_ids\": [";
    out_json << edge_ids[0];
    for (auto i = edge_ids.begin() + 1; i != edge_ids.end(); i++) {
      out_json << ", " << (*i);
    }
    out_json << "]\n    }";
  }
};

class Edge {
 public:
  Edge_id id;
  Vertex_id v1;
  Vertex_id v2;

  Edge() { id = (Edge_id)Numbers::INVALID_ID; }
  Edge(Edge_id id_, Vertex_id v1_, Vertex_id v2_) : id(id_), v1(v1_), v2(v2_) {}

  void to_string(std::ofstream& out_json) const {
    out_json << "{\n      \"id\": " << id << ",\n      \"vertex_ids\": [" << v1
             << ", " << v2 << "]\n    }";
  }
};

class Graph {
 private:
  std::vector<Vertex> vert_mas;
  std::vector<Edge> edge_mas;

 public:
  Graph() {}

  // Возврат векторов
  std::vector<Vertex>& get_vert_mas() { return vert_mas; }
  std::vector<Edge>& get_edge_mas() { return edge_mas; }
  Vertex& get_vertex(int num) { return vert_mas[num]; }

  // Изменение размеров векторов
  void vert_mas_resize(int size) { vert_mas.resize(size); }

  // Добавляет id ребра в вершину
  void add_edge_in_vert(Vertex& x, Edge_id i) { x.edge_ids.push_back(i); }
  void add_edge_in_vert(int num, Edge_id i) {
    vert_mas[num].edge_ids.push_back(i);
  }

  // Добавляет ребро в graph
  void add_edge(Edge_id i, Vertex_id v1, Vertex_id v2) {
    edge_mas.push_back(Edge(i, v1, v2));
  }

  // Добавляет вершину в граф
  void add_vertex(Vertex_id vert_num) { vert_mas[(int)vert_num].id = vert_num; }
  void add_vertex(Vertex_id vert_num, Edge_id edge_i) {
    vert_mas[(int)vert_num].id = vert_num;
    add_edge_in_vert((int)vert_num, edge_i);
  }
};

void output_graph(Graph& graph, int numedge, int numvert) {
  std::ofstream out_json("out.json");
  out_json << "{\n  \"vertices\": [\n    ";
  bool check_first_comma = false;

  for (auto& i : (graph.get_vert_mas())) {
    if (check_first_comma) {
      out_json << ",\n    ";
    }
    check_first_comma = true;
    i.to_string(out_json);
  }

  out_json << "\n  ],\n  \"edges\": [\n    ";
  check_first_comma = false;

  for (auto& i : (graph.get_edge_mas())) {
    if (check_first_comma) {
      out_json << ",\n    ";
    }
    check_first_comma = true;
    i.to_string(out_json);
  }

  out_json << "\n  ]\n}\n";
  out_json.close();
}

int main() {
  const int numedge = 18, numvertex = 14;
  std::vector<Vertex> vert_mas;
  std::vector<Edge> edge_mas;

  std::vector<std::pair<int, int>> graph_data{
      {0, 1},  {0, 2},  {0, 3},  {1, 4},   {1, 5},   {1, 6},
      {2, 7},  {2, 8},  {3, 9},  {4, 10},  {5, 10},  {6, 10},
      {7, 11}, {8, 11}, {9, 12}, {10, 13}, {11, 13}, {12, 13}};

  Graph graph;
  graph.vert_mas_resize(numvertex);

  int i = 0;
  int curr_edge_id = 0;

  for (i = 0; i < numedge; i++) {
    graph.add_edge(i, graph_data[i].first, graph_data[i].second);

    graph.add_vertex(graph_data[i].first, i);
    graph.add_vertex(graph_data[i].second, i);
  }

  output_graph(graph, numedge, numvertex);

  return 0;
}
