#include <algorithm>
#include <cassert>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <iterator>
#include <memory>
#include <vector>

using VertexId = int;
using EdgeId = int;

constexpr int VERTICES_COUNT = 14;

struct Edge {
  const EdgeId id;
  const VertexId vertex1, vertex2;

  Edge(const VertexId& _vertex1, const VertexId& _vertex2, const EdgeId& id_max)
      : id(id_max), vertex1(_vertex1), vertex2(_vertex2) {}
};

struct Vertex {
 public:
  const VertexId id;

  explicit Vertex(const VertexId& id_max) : id(id_max) {}

  bool has_edge_id(const EdgeId& id) const {
    if (edge_ids_.empty())
      return false;
    if (std::find(edge_ids_.begin(), edge_ids_.end(), id) != edge_ids_.end())
      return true;
    return false;
  }

  void add_edge_id(const EdgeId& id) {
    assert(!has_edge_id(id) && "Edge id already exists");
    edge_ids_.push_back(id);
  }

  const std::vector<EdgeId>& get_edge_ids() const { return edge_ids_; }

 private:
  std::vector<EdgeId> edge_ids_;
};

class Graph {
 public:
  bool are_connected(const VertexId& vertex1_id,
                     const VertexId& vertex2_id) const {
    assert(has_vertex(vertex1_id));
    assert(has_vertex(vertex2_id));
    for (const auto& edge1_id : vertices_[vertex1_id].get_edge_ids()) {
      for (const auto& edge2_id : vertices_[vertex2_id].get_edge_ids()) {
        if (edge1_id == edge2_id)
          return true;
      }
    }
    return false;
  }

  bool has_vertex(const VertexId& vertex_id) const {
    for (const auto& vertex : vertices_)
      if (vertex.id == vertex_id)
        return true;
    return false;
  }

  void add_edge(const VertexId& vertex1_id, const VertexId& vertex2_id) {
    assert(has_vertex(vertex1_id));
    assert(has_vertex(vertex2_id));
    assert(!are_connected(vertex1_id, vertex2_id));

    const auto& new_edge =
        edges_.emplace_back(vertex1_id, vertex2_id, get_max_edge_id());
    vertices_[vertex1_id].add_edge_id(new_edge.id);
    vertices_[vertex2_id].add_edge_id(new_edge.id);
  }

  void add_vertex() { vertices_.emplace_back(get_max_vertex_id()); }

  const std::vector<Edge>& get_edges() const { return edges_; }

  const std::vector<Vertex>& get_vertices() const { return vertices_; }

 private:
  std::vector<Edge> edges_;
  std::vector<Vertex> vertices_;
  VertexId vertex_id_max_ = 0;
  EdgeId edge_id_max_ = 0;

  const EdgeId get_max_edge_id() {
    const auto id = edge_id_max_;
    edge_id_max_++;
    return id;
  }

  const VertexId get_max_vertex_id() {
    const auto id = vertex_id_max_;
    vertex_id_max_++;
    return id;
  }
};

class GraphGenerator {
 public:
  Graph build_required_graph() const {
    Graph graph;

    for (int i = 0; i < VERTICES_COUNT; i++) {
      graph.add_vertex();
    }

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

    return graph;
  }
};

class GraphPrinter {
 public:
  std::string vertex_to_json(const Vertex& vertex) const {
    std::string res;
    res += "{\n\t\t\t\"id\": ";
    res += std::to_string(vertex.id);
    res += ",\n\t\t\t\"edge_ids\": [";

    auto edge_ids = vertex.get_edge_ids();

    if (!edge_ids.empty()) {
      for (const auto& edge_id : edge_ids) {
        res += std::to_string(edge_id);
        res += ", ";
      }
      res.pop_back();
      res.pop_back();
    }
    res += "]\n\t\t}";

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

  std::string to_json(const Graph& graph) const {
    std::string res;
    res += "{\n\t \"vertices\": [\n\t\t";

    auto vertices = graph.get_vertices();

    if (!vertices.empty()) {
      for (const auto& vertex : graph.get_vertices()) {
        res += vertex_to_json(vertex);
        res += ", ";
      }
      res.pop_back();
      res.pop_back();
    }

    res += "\n\t],\n\t\"edges\": [\n\t\t";

    auto edges = graph.get_edges();

    if (!edges.empty()) {
      for (const auto& edge : graph.get_edges()) {
        res += edge_to_json(edge);
      }
      res.pop_back();
      res.pop_back();
    }

    res += "\n\t]\n}\n";

    return res;
  }
};

int main() {
  const GraphGenerator generator;
  Graph graph = generator.build_required_graph();

  const GraphPrinter printer;
  std::ofstream myfile;
  myfile.open("graph.json");
  myfile << printer.to_json(graph);
  myfile.close();

  return 0;
}
