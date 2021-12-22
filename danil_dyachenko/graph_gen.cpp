#include <array>
#include <cassert>
#include <fstream>
#include <iostream>
#include <set>
#include <vector>

using VertexId = int;
using EdgeId = int;

constexpr int INVALID_ID = -1;

class Graph {
 public:
  struct Vertex {
    explicit Vertex(const VertexId& _id) : id_(_id) {}

    VertexId id() const { return id_; }
    std::set<EdgeId> edges_ids() const { return edges_ids_; }

   private:
    const VertexId id_ = INVALID_ID;
    const std::set<EdgeId> edges_ids_ = {};
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
  Vertex& add_vertex() {
    Vertex new_vertex(current_vertex_id);
    vertices_.push_back(new_vertex);
    current_vertex_id++;
    return vertices_[current_vertex_id - 1];
  }

  Edge& add_edge(const VertexId& from_vertex_id, const VertexId& to_vertex_id) {
    assert(from_vertex_id < current_vertex_id && from_vertex_id >= 0);
    assert(to_vertex_id < current_vertex_id && to_vertex_id >= 0);
    Edge new_edge(current_edge_id, from_vertex_id, to_vertex_id);
    vertices_[to_vertex_id].edges_ids().insert(from_vertex_id);
    vertices_[from_vertex_id].edges_ids().insert(to_vertex_id);
    current_edge_id++;
    edges_.push_back(new_edge);
    return edges_[current_edge_id - 1];
  }

  std::vector<Vertex> vertices() const { return vertices_; }
  std::vector<Edge> edges() const { return edges_; }

 private:
  std::vector<Vertex> vertices_;
  std::vector<Edge> edges_;
  EdgeId current_edge_id = 0;
  VertexId current_vertex_id = 0;
};

namespace printing {

std::string print_vertex(const Graph::Vertex& vertex) {
  std::string string;
  string += "{ \"id\": ";
  string += std::to_string(vertex.id());
  string += ", \"edge_ids\": [";
  auto edges_ids = vertex.edges_ids();
  for (auto i = edges_ids.begin(); i != edges_ids.end(); i++) {
    string += i != edges_ids.begin() ? ", " : "";
    string += std::to_string(*i);
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
  for (auto i = vertices.begin(); i != vertices.end() - 1; i++) {
    string += "    " + print_vertex(*i) + ",\n";
  }
  string += "    " + print_vertex(*(vertices.end() - 1)) + "\n" +
            "  ],\n  \"edges\": [\n";
  for (auto i = edges.begin(); i != edges.end() - 1; i++) {
    string += "    " + print_edge(*i) + ",\n";
  }
  string += "    " + print_edge(*(edges.end() - 1)) + "\n" + "  ]\n}\n";
  return string;
}

namespace json {
void write_file(const std::string& string, const std::string& name) {
  std::ofstream file;
  file.open(name, std::ios::out);
  file << string;
}
}  // namespace json
}  // namespace printing

constexpr int kVerticesCount = 14;

int main(void) {
  auto my_graph = Graph();
  for (int i = 0; i < kVerticesCount; i++) {
    my_graph.add_vertex();
  }
  my_graph.add_edge(0, 1);
  my_graph.add_edge(0, 2);
  my_graph.add_edge(0, 3);
  my_graph.add_edge(1, 4);
  my_graph.add_edge(1, 5);
  my_graph.add_edge(1, 6);
  my_graph.add_edge(2, 7);
  my_graph.add_edge(2, 8);
  my_graph.add_edge(3, 9);
  my_graph.add_edge(4, 10);
  my_graph.add_edge(5, 10);
  my_graph.add_edge(6, 10);
  my_graph.add_edge(7, 11);
  my_graph.add_edge(8, 11);
  my_graph.add_edge(9, 12);
  my_graph.add_edge(10, 13);
  my_graph.add_edge(11, 13);
  my_graph.add_edge(12, 13);
  printing::json::write_file(printing::print_graph(my_graph), "my_graph.json");
  return 0;
}