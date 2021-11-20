#include <array>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

using std::array;
using std::string;
using std::to_string;
using std::vector;
using VertexId = int;
using EdgeId = int;
constexpr int kVerticesCount = 14;

struct Vertex {
 public:
  const VertexId id;

  explicit Vertex(const VertexId& _id) : id(_id) {}

  void add_edge_id(const EdgeId& id) { edge_ids_.push_back(id); }

  string print() const {
    string vertex_output =
        "\"id\": " + to_string(id) + ",\n\t\t\t\"edge_ids\": [";
    for (const auto& edge_id : edge_ids_) {
      vertex_output += to_string(edge_id);
      if (edge_id != edge_ids_.back()) {
        vertex_output += ", ";
      }
    }
    vertex_output += "]";
    return vertex_output;
  }

 private:
  vector<int> edge_ids_;
};

struct Edge {
  const int from_vertex_id_;
  const int to_vertex_id_;
  const EdgeId id;

  Edge(const EdgeId& _id,
       const VertexId& _from_vertex_id,
       const VertexId& _to_vertex_id)
      : id(_id),
        from_vertex_id_(_from_vertex_id),
        to_vertex_id_(_to_vertex_id) {}

  string print() const {
    return "\"id\": " + std::to_string(id) + ",\n\t\t\t\"vertex_ids\": [" +
           to_string(from_vertex_id_) + ", " + to_string(to_vertex_id_) + "]";
  }
};

class Graph {
 public:
  void add_vertex() {
    const VertexId new_vertex_id = get_new_vertex_id();
    vertices_.emplace(new_vertex_id, new_vertex_id);
  }

  void add_edge(const VertexId& from_vertex_id, const VertexId& to_vertex_id) {
    const EdgeId new_edge_id = get_new_edge_id();
    edges_.emplace_back(new_edge_id, from_vertex_id, to_vertex_id);
    vertices_.at(from_vertex_id).add_edge_id(new_edge_id);
    vertices_.at(to_vertex_id).add_edge_id(new_edge_id);
  };

  string print() const {
    string graph_output = "{\n\t\"vertices\": [\n\t\t";
    for (auto it = vertices_.begin(); it != vertices_.end(); ++it) {
      graph_output += "{\n\t\t\t" + it->second.print() + "\n\t\t}";
      if (std::next(it) != vertices_.end()) {
        graph_output += ",\n\t\t";
      } else {
        graph_output += "\n\t";
      }
    }
    graph_output += "],\n\t\"edges\": [\n\t\t";
    for (const auto& edge : edges_) {
      graph_output += "{\n\t\t\t" + edge.print() + "\n\t\t}";
      if (edge.id != edges_.back().id) {
        graph_output += ",\n\t\t";
      } else {
        graph_output += "\n\t";
      }
    }
    graph_output += "]\n}\n";
    return graph_output;
  }

 private:
  std::unordered_map<VertexId, Vertex> vertices_;
  vector<Edge> edges_;
  int vertex_id_counter_ = 0;
  int edge_id_counter_ = 0;

  VertexId get_new_vertex_id() { return vertex_id_counter_++; }

  EdgeId get_new_edge_id() { return edge_id_counter_++; }
};

int main() {
  auto graph = Graph();

  for (int i = 0; i < kVerticesCount; i++) {
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

  std::ofstream graph_file;
  graph_file.open("graph.json");
  graph_file << graph.print();
}
