#include <assert.h>
#include <stdlib.h>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

constexpr int INVALID_ID = -1;

using VertexId = int;
using EdgeId = int;

class Vertex {
 public:
  Vertex(VertexId id_ = INVALID_ID) : id(id_) {}

  // Возврат значений
  VertexId get_id() const { return id; }
  EdgeId get_edge_id(EdgeId i) const { return edge_ids[i]; }
  const std::vector<EdgeId>& get_edge_ids_vector() const { return edge_ids; }

  // Добавить id ребра в edge_ids
  void add_edge_id(EdgeId i) { edge_ids.emplace_back(i); }

 private:
  VertexId id;
  std::vector<EdgeId> edge_ids;
};

class Edge {
 public:
  Edge() : id(INVALID_ID) {}
  Edge(VertexId v1_, VertexId v2_, EdgeId id_ = INVALID_ID)
      : v1(v1_), v2(v2_), id(id_) {}

  // Возврат значений
  EdgeId get_id() const { return id; }
  VertexId get_vertex1_id() const { return v1; }
  VertexId get_vertex2_id() const { return v2; }

 private:
  EdgeId id;
  VertexId v1;
  VertexId v2;
};

class Graph {
 public:
  Graph() : vert_num(0), edge_num(0) {}

  // Добавляет ребро в graph
  void add_edge(VertexId v1, VertexId v2) {
    edge_mas.emplace_back(v1, v2, edge_num);
    vert_mas[v1].add_edge_id(edge_num);
    vert_mas[v2].add_edge_id(edge_num);
    edge_num++;
  }

  // Добавляет вершину в граф
  void add_vertex() {
    vert_mas.emplace_back(vert_num);
    vert_num++;
  }

  // Возврат векторов
  const std::vector<Vertex>& get_vert_mas() const { return vert_mas; }
  const std::vector<Edge>& get_edge_mas() const { return edge_mas; }

 private:
  VertexId vert_num;
  EdgeId edge_num;
  std::vector<Vertex> vert_mas;
  std::vector<Edge> edge_mas;
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

      result << "{\n      \"id\": " << vertex.get_id()
             << ",\n      \"edge_ids\": [";

      check_first_comma = false;
      for (auto& i : vertex.get_edge_ids_vector()) {
        if (check_first_comma) {
          result << ", ";
        }
        check_first_comma = true;
        result << i;
      }

      result << "]\n    }";

      check_first_comma = true;
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
  Graph graph;

  for (int i = 0; i < numvertex; i++) {
    graph.add_vertex();
  }

  std::vector<std::pair<int, int>> graph_data{
      {0, 1},  {0, 2},  {0, 3},  {1, 4},   {1, 5},   {1, 6},
      {2, 7},  {2, 8},  {3, 9},  {4, 10},  {5, 10},  {6, 10},
      {7, 11}, {8, 11}, {9, 12}, {10, 13}, {11, 13}, {12, 13}};

  for (int edge_num = 0; edge_num < numedge; edge_num++) {
    graph.add_edge(graph_data[edge_num].first, graph_data[edge_num].second);
  }

  GraphPrinter output_graph;

  std::ofstream out_json("out.json");
  out_json << output_graph.print(graph);
  out_json.close();

  return 0;
}
