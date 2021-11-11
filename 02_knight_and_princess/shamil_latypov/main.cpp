#include <assert.h>
#include <stdlib.h>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

enum class Numbers { INVALID_ID = -1 };

using Vertex_id = int;
using Edge_id = int;

class Vertex {
 private:
  Vertex_id id;

 public:
  std::vector<Edge_id> edge_ids;

  Vertex() : id((Vertex_id)Numbers::INVALID_ID) {}
  Vertex(Vertex_id id_) : id(id_) {}

  Vertex_id get_id() const { return id; }
  void set_id(Vertex_id v_id) { id = v_id; }

  Edge_id get_edge_id(Edge_id i) { return edge_ids[i]; }
};

class Edge {
 private:
  Edge_id id;
  Vertex_id v1;
  Vertex_id v2;

 public:
  Edge() : id((Edge_id)Numbers::INVALID_ID) {}
  Edge(Edge_id id_, Vertex_id v1_, Vertex_id v2_) : id(id_), v1(v1_), v2(v2_) {}

  Edge_id get_id() const { return id; }
  Vertex_id get_vertex1_id() const { return v1; }
  Vertex_id get_vertex2_id() const { return v2; }
};

class Graph {
 private:
  std::vector<Vertex> vert_mas;
  std::vector<Edge> edge_mas;

 public:
  Graph() {}
  // Изменение размеров векторов
  void vert_mas_resize(int size) { vert_mas.resize(size); }

  // Добавляет id ребра в вершину
  void add_edge_in_vert(int num, Edge_id i) {
    vert_mas[num].edge_ids.emplace_back(i);
  }

  // Добавляет ребро в graph
  void add_edge(Edge_id i, Vertex_id v1, Vertex_id v2) {
    edge_mas.emplace_back(i, v1, v2);

    add_edge_in_vert(v1, i);
    add_edge_in_vert(v2, i);
  }

  // Добавляет вершину в граф
  void add_vertex() { vert_mas.push_back(Vertex()); }
  void add_vertex(Vertex_id vert_num) {
    vert_mas[(int)vert_num].set_id(vert_num);
  }

  // Возврат векторов
  const std::vector<Vertex>& get_vert_mas() const { return vert_mas; }
  const std::vector<Edge>& get_edge_mas() const { return edge_mas; }
};

class GraphPrinter {
 public:
  std::string print(const Graph& graph) const {
    std::stringstream result;
    result << "{\n  \"vertices\": [\n    ";
    bool check_first_comma = false;

    for (auto& vertex : graph.get_vert_mas()) {
      if (check_first_comma) {
        result << ",\n    ";
      }
      check_first_comma = true;

      result << "{\n      \"id\": " << vertex.get_id()
             << ",\n      \"edge_ids\": [";
      result << vertex.edge_ids[0];
      for (auto i = vertex.edge_ids.begin() + 1; i != vertex.edge_ids.end();
           i++) {
        result << ", " << (*i);
      }
      result << "]\n    }";
    }

    result << "\n  ],\n  \"edges\": [\n    ";
    check_first_comma = false;

    for (auto& edge : graph.get_edge_mas()) {
      if (check_first_comma) {
        result << ",\n    ";
      }
      check_first_comma = true;

      result << "{\n      \"id\": " << edge.get_id()
             << ",\n      \"vertex_ids\": [" << edge.get_vertex1_id() << ", "
             << edge.get_vertex2_id() << "]\n    }";
    }

    result << "\n  ]\n}\n";
    return result.str();
  }
};

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

  int edge_num = 0;

  for (edge_num = 0; edge_num < numedge; edge_num++) {
    graph.add_edge(edge_num, graph_data[edge_num].first,
                   graph_data[edge_num].second);

    graph.add_vertex(graph_data[edge_num].first);
    graph.add_vertex(graph_data[edge_num].second);
  }

  GraphPrinter output_graph;

  std::ofstream out_json("out.json");
  out_json << output_graph.print(graph);

  return 0;
}
