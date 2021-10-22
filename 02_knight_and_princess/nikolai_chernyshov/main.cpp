#include <cstdlib>
#include <fstream>
#include <iostream>
#include <iterator>
#include <memory>
#include <vector>

using VertexId = int;
using EdgeId = int;

struct Edge {
  const EdgeId id;
  const VertexId vertex1, vertex2;

  Edge(VertexId _vertex1, VertexId _vertex2, EdgeId id_max)
      : id(id_max), vertex1(_vertex1), vertex2(_vertex2) {}
};

struct Vertex {
  const VertexId id;
  const std::vector<EdgeId> edges;

  Vertex(const std::vector<EdgeId>& _edges, VertexId id_max)
      : id(id_max), edges(_edges) {}
};

class Graph {
 private:
  std::vector<Edge> edges_;
  std::vector<Vertex> vertices_;
  VertexId vertex_id_max_ = 0;
  EdgeId edge_id_max_ = 0;

 public:
  void add_edge(VertexId vertex1, VertexId vertex2) {
    edges_.emplace_back(vertex1, vertex2, edge_id_max_);
    edge_id_max_++;
  }

  void add_vertex(const std::vector<EdgeId>& edges) {
    vertices_.emplace_back(edges, vertex_id_max_);
    vertex_id_max_++;
  }

  const std::vector<Edge>& get_edges() { return edges_; }

  const std::vector<Vertex>& get_vertices() { return vertices_; }
};

class GraphGenerator {
 public:
  void required_graph(Graph& graph)  // build required graph
  {
    graph.add_vertex({0, 1, 2});
    graph.add_vertex({0, 3, 4, 5});
    graph.add_vertex({1, 6, 7});
    graph.add_vertex({2, 8});
    graph.add_vertex({3, 9});
    graph.add_vertex({4, 10});
    graph.add_vertex({5, 11});
    graph.add_vertex({6, 12});
    graph.add_vertex({7, 13});
    graph.add_vertex({8, 14});
    graph.add_vertex({9, 10, 11, 15});
    graph.add_vertex({12, 13, 16});
    graph.add_vertex({14, 17});
    graph.add_vertex({15, 16, 17});

    graph.add_edge(0, 1);
    graph.add_edge(0, 2);
    graph.add_edge(0, 3);
    graph.add_edge(1, 4);
    graph.add_edge(1, 5);
    graph.add_edge(1, 6);
    graph.add_edge(2, 7);
    graph.add_edge(2, 8);
    graph.add_edge(3, 9);
    graph.add_edge(4, 10);
    graph.add_edge(5, 10);
    graph.add_edge(6, 10);
    graph.add_edge(7, 11);
    graph.add_edge(8, 11);
    graph.add_edge(9, 12);
    graph.add_edge(10, 13);
    graph.add_edge(11, 13);
    graph.add_edge(12, 13);
  }
};

class GraphPrinter {
 public:
  std::string vertex_to_json(const Vertex& vertex) const {
    std::string res;
    res += "{\n\t\t\t\"id\": ";
    res += std::to_string(vertex.id);
    res += ",\n\t\t\t\"edge_ids\": [";

    for (const auto& edge : vertex.edges) {
      res += std::to_string(edge);
      res += ", ";
    }
    res.pop_back();
    res.pop_back();
    res += "]\n\t\t}, ";

    return res;
  }

  std::string edge_to_json(const Edge& edge) const {
    std::string res;

    res += "{\n\t\t\t\"id\": ";
    res += std::to_string(edge.id);
    res += ",\n\t\t\t\"vertex_ids\": [";
    res += std::to_string(edge.vertex1);
    res += ", ";
    res += std::to_string(edge.vertex2);
    res += "]\n\t\t}, ";

    return res;
  }

  std::string to_json(Graph& graph) const {
    std::string res;
    res += "{\n\t \"vertices\": [\n\t\t";

    for (const auto& vertex : graph.get_vertices()) {
      res += vertex_to_json(vertex);
    }
    res.pop_back();
    res.pop_back();
    res += "\n\t],\n\t\"edges\": [\n\t\t";

    for (const auto& edge : graph.get_edges()) {
      res += edge_to_json(edge);
    }
    res.pop_back();
    res.pop_back();
    res += "\n\t]\n}\n";

    return res;
  }
};

int main() {
  Graph graph;

  GraphGenerator generator;
  generator.required_graph(graph);

  GraphPrinter printer;
  std::ofstream myfile;
  myfile.open("graph.json");
  myfile << printer.to_json(graph);
  myfile.close();

  return 0;
}
