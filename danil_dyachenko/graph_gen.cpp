#include <cassert>
#include <fstream>
#include <iostream>
#include <set>
#include <unordered_map>
#include <vector>

constexpr int kVerticesCount = 14;

class Graph {
 public:
  using VertexId = int;
  using EdgeId = int;

  static constexpr int INVALID_ID = -1;

  struct Vertex {
    explicit Vertex(const VertexId& _id) : id_(_id) {}

    VertexId id() const { return id_; }

   private:
    const VertexId id_ = INVALID_ID;
  };

  struct Edge {
    Edge(const EdgeId& _id,
         const VertexId& _from_vertex_id,
         const VertexId& _to_vertex_id)
        : id_(_id),
          from_vertex_id_(_from_vertex_id),
          to_vertex_id_(_to_vertex_id) {}
    EdgeId id() const { return id_; }
    VertexId from_vertex_id() const { return from_vertex_id_; }
    VertexId to_vertex_id() const { return to_vertex_id_; }

   private:
    const EdgeId id_ = INVALID_ID;
    const VertexId from_vertex_id_ = INVALID_ID;
    const VertexId to_vertex_id_ = INVALID_ID;
  };

  // Graph functions
  const VertexId get_vertex_id() { return current_vertex_id++; }
  const EdgeId get_edge_id() { return current_edge_id++; }

  Vertex& add_vertex() {
    const VertexId new_vertex_id = get_vertex_id();
    adjacency_list_[new_vertex_id] = {};
    return vertices_.emplace_back(new_vertex_id);
  }

  Edge& add_edge(const VertexId& from_vertex_id, const VertexId& to_vertex_id) {
    assert(from_vertex_id < current_vertex_id);
    assert(from_vertex_id >= 0);
    assert(to_vertex_id < current_vertex_id);
    assert(to_vertex_id >= 0);
    const EdgeId new_edge_id = get_edge_id();
    adjacency_list_[to_vertex_id].emplace(new_edge_id);
    adjacency_list_[from_vertex_id].emplace(new_edge_id);
    return edges_.emplace_back(current_edge_id, from_vertex_id, to_vertex_id);
  }

  const std::set<EdgeId>& get_edges_ids(const VertexId vertex_id) const {
    return adjacency_list_.at(vertex_id);
  }

  const std::vector<Vertex>& vertices() const { return vertices_; }
  const std::vector<Edge>& edges() const { return edges_; }

 private:
  std::unordered_map<VertexId, std::set<EdgeId>> adjacency_list_;
  std::vector<Vertex> vertices_;
  std::vector<Edge> edges_;
  EdgeId current_edge_id = 0;
  VertexId current_vertex_id = 0;
};

namespace printing {
namespace json {
std::string print_vertex(const Graph::Vertex& vertex, const Graph& graph) {
  std::string string;
  string += "{ \"id\": ";
  string += std::to_string(vertex.id());
  string += ", \"edge_ids\": [";
  auto edges_ids = graph.get_edges_ids(vertex.id());
  for (auto edge_id_it = edges_ids.begin(); edge_id_it != edges_ids.end();
       edge_id_it++) {
    if (edge_id_it != edges_ids.begin())
      string += ", ";
    string += std::to_string(*edge_id_it);
  }
  string += "] }";
  return string;
}

std::string print_edge(const Graph::Edge& edge) {
  std::string string;
  string += "{ \"id\": ";
  string += std::to_string(edge.id());
  string += ", \"vertex_ids\": [";
  string += std::to_string(edge.from_vertex_id());
  string += ", ";
  string += std::to_string(edge.to_vertex_id());
  string += "] }";
  return string;
}

std::string print_graph(const Graph& graph) {
  auto vertices = graph.vertices();
  assert(vertices.size() != 0);
  auto edges = graph.edges();
  assert(edges.size() != 0);
  std::string string;
  string += "{\n  \"vertices\": [\n";
  for (const auto& vertex : vertices) {
    string += "    " + print_vertex(vertex, graph) + ",\n";
  }
  if (!vertices.empty()) {
    string.pop_back();
    string.pop_back();
  }
  string += "\n  ],\n  \"edges\": [\n";
  for (const auto& edge : edges) {
    string += "    " + print_edge(edge) + ",\n";
  }
  if (!edges.empty()) {
    string.pop_back();
    string.pop_back();
  }
  string += "\n  ]\n}\n";
  return string;
}
}  // namespace json
}  // namespace printing

void write_file(const std::string& string, const std::string& name) {
  std::ofstream file;
  file.open(name, std::ios::out);
  file << string;
}

Graph generate_graph() {
  Graph new_graph = Graph();
  for (int i = 0; i < kVerticesCount; i++) {
    new_graph.add_vertex();
  }
  new_graph.add_edge(0, 1);
  new_graph.add_edge(0, 2);
  new_graph.add_edge(0, 3);
  new_graph.add_edge(1, 4);
  new_graph.add_edge(1, 5);
  new_graph.add_edge(1, 6);
  new_graph.add_edge(2, 7);
  new_graph.add_edge(2, 8);
  new_graph.add_edge(3, 9);
  new_graph.add_edge(4, 10);
  new_graph.add_edge(5, 10);
  new_graph.add_edge(6, 10);
  new_graph.add_edge(7, 11);
  new_graph.add_edge(8, 11);
  new_graph.add_edge(9, 12);
  new_graph.add_edge(10, 13);
  new_graph.add_edge(11, 13);
  new_graph.add_edge(12, 13);
  return new_graph;
}

int main(void) {
  const auto my_graph = generate_graph();
  write_file(printing::json::print_graph(my_graph), "my_graph.json");
  return 0;
}
