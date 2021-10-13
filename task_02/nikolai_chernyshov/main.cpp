#include <cstdlib>
#include <fstream>
#include <iostream>
#include <iterator>
#include <memory>
#include <vector>

using VertexId = int;
using EdgeId = int;

struct Edge {
  inline static EdgeId id_max;
  EdgeId id;
  VertexId vertex1, vertex2 = -1; // -1 if doesn't exist

  Edge(VertexId vertex1_, VertexId vertex2_ = -1)
      : id(id_max), vertex1(vertex1_), vertex2(vertex2_) {
    id_max++;
  }
};

struct Vertex {
  inline static VertexId id_max;
  VertexId id;
  std::vector<EdgeId> edges;

  Vertex(EdgeId parent_edge_id, std::vector<EdgeId> edges_ids) : id(id_max) {
    id_max++;
    if (parent_edge_id != -1) { // -1 if root vertex -> no parent
      edges.push_back(parent_edge_id);
    }
    if (!(edges_ids.empty())) {
      edges.insert(edges.end(), edges_ids.begin(), edges_ids.end());
    }
  }
};

class Graph {
public:
  std::vector<Edge> edges;
  std::vector<Vertex> vertices;

  Graph() // build required graph
  {
    Vertex::id_max = 0;
    Edge::id_max = 0;

    std::vector<EdgeId> vect;

    vect = {0, 1, 2};
    vertices.push_back(Vertex(-1, vect));
    vect = {3, 4, 5};
    vertices.push_back(Vertex(0, vect));
    vect = {6, 7};
    vertices.push_back(Vertex(1, vect));
    vect = {8};
    vertices.push_back(Vertex(2, vect));
    vect = {9};
    vertices.push_back(Vertex(3, vect));
    vect = {10};
    vertices.push_back(Vertex(4, vect));
    vect = {11};
    vertices.push_back(Vertex(5, vect));
    vect = {12};
    vertices.push_back(Vertex(6, vect));
    vect = {13};
    vertices.push_back(Vertex(7, vect));
    vect = {14};
    vertices.push_back(Vertex(8, vect));
    vect = {10, 11, 15};
    vertices.push_back(Vertex(9, vect));
    vect = {13, 16};
    vertices.push_back(Vertex(12, vect));
    vect = {17};
    vertices.push_back(Vertex(14, vect));
    vect = {16, 17};
    vertices.push_back(Vertex(15, vect));

    edges.push_back(Edge(0, 1));
    edges.push_back(Edge(0, 2));
    edges.push_back(Edge(0, 3));
    edges.push_back(Edge(1, 4));
    edges.push_back(Edge(1, 5));
    edges.push_back(Edge(1, 6));
    edges.push_back(Edge(2, 7));
    edges.push_back(Edge(2, 8));
    edges.push_back(Edge(3, 9));
    edges.push_back(Edge(4, 10));
    edges.push_back(Edge(5, 10));
    edges.push_back(Edge(6, 10));
    edges.push_back(Edge(7, 11));
    edges.push_back(Edge(8, 11));
    edges.push_back(Edge(9, 12));
    edges.push_back(Edge(10, 13));
    edges.push_back(Edge(11, 13));
    edges.push_back(Edge(12, 13));
  }
};

class GraphPrinter {
public:
  std::string vertex_to_json(const Vertex &vertex) const {
    std::string res;
    res += "{\n\t\t\t\"id\": ";
    res += std::to_string(vertex.id);
    res += ",\n\t\t\t\"edge_ids\": [";

    // for (auto it1 = it.edges.begin(); it1 != it.edges.end(); it1++) {
    for (const auto &it1 : vertex.edges) {
      res += std::to_string(it1);
      res += ", ";
    }
    res.pop_back();
    res.pop_back();
    res += "]\n\t\t}, ";

    return res;
  }

  std::string edge_to_json(const Edge &edge) const {
    std::string res;
    res += "{\n\t\t\t\"id\": ";
    res += std::to_string(edge.id);
    res += ",\n\t\t\t\"vertex_ids\": [";
    res += std::to_string(edge.vertex1);
    if (edge.vertex2 != -1) {
      res += ", ";
      res += std::to_string(edge.vertex2);
    }
    res += "]\n\t\t}, ";

    return res;
  }

  std::string to_json(const Graph &graph) const {
    std::string res;
    res += "{\n\t \"vertices\": [\n\t\t";

    // for (auto it = vertices.begin(); it != vertices.end(); it++) {
    for (const auto &it : graph.vertices) {
      res += vertex_to_json(it);
    }
    res.pop_back();
    res.pop_back();
    res += "\n\t],\n\t\"edges\": [\n\t\t";

    // for (auto it = edges.begin(); it != edges.end(); it++) {
    for (const auto &it : graph.edges) {
      res += edge_to_json(it);
    }
    res.pop_back();
    res.pop_back();
    res += "\n\t]\n}\n";

    return res;
  }
};

int main() {
  Graph graph;
  GraphPrinter printer;
  std::ofstream myfile;
  myfile.open("graph.json");
  myfile << printer.to_json(graph);
  myfile.close();
  return 0;
}
