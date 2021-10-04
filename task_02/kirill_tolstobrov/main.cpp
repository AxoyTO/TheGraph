#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iostream>
#include <iterator>
#include <map>
#include <utility>
#include <vector>

class Vertex {
  int id;

 public:
  explicit Vertex(int init_id = 0) : id(init_id) {}
  int get_id() const { return id; }
};

class Edge {
  int id;
  int vertex1_id;
  int vertex2_id;

 public:
  Edge(int init_id, int v1_init, int v2_init)
      : id(init_id), vertex1_id(v1_init), vertex2_id(v2_init){};
  int get_id() const { return id; }
  int get_v1_id() const { return vertex1_id; }
  int get_v2_id() const { return vertex2_id; }
};

class Graph {
  std::vector<Edge> edges;
  std::vector<Vertex> vertices;

  // connections_map: vertex1 -> edge -> vertex2
  std::map<int, std::map<int, int>> connections_map;

  int vertices_amount;
  int edges_amount;

 public:
  Graph() : vertices_amount(0), edges_amount(0) {}

  int get_vertices_amount() const { return vertices_amount; }
  int get_edges_amount() const { return edges_amount; }

  void add_new_vertex() {
    vertices.push_back(Vertex(vertices_amount));
    vertices_amount++;
  }

  void bind_vertices(int id1, int id2) {
    edges.push_back(Edge(edges_amount, id1, id2));
    edges_amount++;
    connections_map[id1][edges_amount - 1] = id2;
    connections_map[id2][edges_amount - 1] = id1;
  }

  operator std::string() {
    std::string result = "{\n  \"vertices\": [\n";

    for (int i = 0; i < vertices_amount; i++) {
      std::string vertex_string = "";
      vertex_string += "    {\n      \"id\": " + std::to_string(i);
      vertex_string += ",\n      \"edge_ids\": ";

      auto& cur_vertex = connections_map[i];
      for (auto it = cur_vertex.begin(); it != cur_vertex.end(); it++) {
        if (it == cur_vertex.begin()) {
          vertex_string += "[";
        } else {
          vertex_string += ", ";
        }
        vertex_string += std::to_string(it->first);
      }
      vertex_string += "]\n    }";

      result += vertex_string;
      if (i != vertices_amount - 1)
        result += ",\n";
    }

    result += "\n  ],\n  \"edges\": [\n";

    for (int i = 0; i < edges_amount; i++) {
      std::string edge_string = "";
      edge_string += "    {\n      \"id\": " + std::to_string(i);
      edge_string += ",\n      \"vertex_ids\": ";

      edge_string += "[" + std::to_string(edges[i].get_v1_id());
      edge_string += ", " + std::to_string(edges[i].get_v2_id());
      edge_string += "]\n    }";

      result += edge_string;
      if (i != edges_amount - 1)
        result += ",\n";
    }

    result += "\n  ]\n}";
    return result;
  }
};

void generateGraph(Graph& g, int max_vertices = 10, int max_edges = 15) {
  enum { PROBABILITY_COEF = 100 };

  srand(std::time(NULL));

  for (int i = 0; i < max_vertices; i++) {
    const float new_vertex_prob =
        PROBABILITY_COEF -
        (float)PROBABILITY_COEF * g.get_vertices_amount() / max_vertices;
    if (((double)std::rand() / RAND_MAX) * PROBABILITY_COEF < new_vertex_prob) {
      g.add_new_vertex();
      if (g.get_vertices_amount() == 1) {
        continue;
      }

      int vertex_to_bind = std::rand() % (g.get_vertices_amount() - 1);
      g.bind_vertices(g.get_vertices_amount() - 1, vertex_to_bind);
    }
  }

  for (int i = g.get_edges_amount(); i < max_edges; i++) {
    float new_edge_prob = PROBABILITY_COEF - (float)PROBABILITY_COEF *
                                                 g.get_edges_amount() /
                                                 max_edges;
    if (((double)std::rand() / RAND_MAX) * PROBABILITY_COEF < new_edge_prob) {
      int vertex_to_bind1 = std::rand() % g.get_vertices_amount();
      int vertex_to_bind2 = std::rand() % g.get_vertices_amount();
      g.bind_vertices(vertex_to_bind1, vertex_to_bind2);
    }
  }
}

void generateGraph(Graph& g,
                   int vert_number,
                   std::vector<std::pair<int, int>> connections) {
  for (int i = 0; i < vert_number; i++) {
    g.add_new_vertex();
  }
  for (int i = 0; i < connections.size(); i++) {
    g.bind_vertices(connections[i].first, connections[i].second);
  }
}

int main() {
  Graph g;

  std::vector<std::pair<int, int>> g_connections{
      {0, 1},  {0, 2},  {0, 3},  {1, 4},   {1, 5},   {1, 6},
      {2, 7},  {2, 8},  {3, 9},  {4, 10},  {5, 10},  {6, 10},
      {7, 11}, {8, 11}, {9, 12}, {10, 13}, {11, 13}, {12, 13}};

  generateGraph(g, 14, g_connections);

  std::ofstream file;
  file.open("graph.json");

  if (file.is_open()) {
    file << (std::string)g << std::endl;
  }

  return 0;
}

