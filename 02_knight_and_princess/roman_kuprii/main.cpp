#include <array>
#include <cassert>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using std::endl;
using std::to_string;
using std::vector;

using EdgeId = int;
using VertexId = int;

constexpr int INVALID_ID = -1;
const std::string JSON_GRAPH_FILENAME = "graph.json";

struct Edge {
  const EdgeId id = INVALID_ID;
  const std::array<VertexId, 2> connected_vertices;

  Edge(const VertexId& start, const VertexId& end, const EdgeId& _id)
      : id(_id), connected_vertices({start, end}) {}

  std::string to_json() const {
    std::string res;
    res = "{ \"id\": ";
    res += to_string(id);
    res += ", \"vertex_ids\": [";
    res += to_string(connected_vertices[0]);
    res += ", ";
    res += to_string(connected_vertices[1]);
    res += "] }";
    return res;
  }
};

bool is_edge_id_included(const EdgeId& id, const vector<EdgeId>& edge_ids) {
  for (const auto& edge_id : edge_ids)
    if (id == edge_id)
      return true;
  return false;
}

struct Vertex {
 public:
  const VertexId id = INVALID_ID;

  explicit Vertex(const VertexId& _id) : id(_id) {}

  std::string to_json() const {
    std::string res;
    res = "{ \"id\": ";
    res += to_string(id) + ", \"edge_ids\": [";
    for (const auto& edge_id : edges_ids_) {
      res += to_string(edge_id);
      res += ", ";
    }
    res.pop_back();
    res.pop_back();
    res += "] }";
    return res;
  }

  void add_edge_id(const EdgeId& _id) {
    assert(is_edge_id_included(_id, edges_ids_) == false);
    edges_ids_.push_back(_id);
  }

 private:
  std::vector<EdgeId> edges_ids_;
};

bool is_vertex_id_included(const std::vector<Vertex>& vertices,
                           const VertexId& id) {
  for (const auto& vert : vertices)
    if (vert.id == id)
      return (true);
  return false;
}

bool edge_connection_check(const std::vector<Edge>& edges,
                           const VertexId& out_id,
                           const VertexId& dest_id) {
  for (const auto& edge : edges) {
    if (edge.connected_vertices[0] == out_id &&
        edge.connected_vertices[1] == dest_id)
      return true;
  }
  return false;
}

class Graph {
 public:
  std::string to_json() const {
    std::string res;
    res = "{ \"vertices\": [ ";
    for (const auto& v_it : vertices_) {
      res += v_it.to_json();
      res += ", ";
    }
    res.pop_back();
    res.pop_back();
    res += " ], \"edges\": [ ";
    for (const auto& e_it : edges_) {
      res += e_it.to_json();
      res += ", ";
    }
    res.pop_back();
    res.pop_back();
    res += " ] }\n";
    return res;
  }

  void add_vertex() { vertices_.emplace_back(vertices_.size()); }

  void connect_vertices(const VertexId& out_id, const VertexId& dest_id) {
    // check if vertices exist
    assert(is_vertex_id_included(vertices_, out_id));
    assert(is_vertex_id_included(vertices_, dest_id));

    // check if they are not connected
    assert(edge_connection_check(edges_, out_id, dest_id) == 0);

    EdgeId id = edges_.size();
    edges_.emplace_back(out_id, dest_id, id);

    // add information into Verex structure
    vertices_[out_id].add_edge_id(id);
    vertices_[dest_id].add_edge_id(id);
  }

 private:
  vector<Vertex> vertices_;
  vector<Edge> edges_;
};

void write_graph(const Graph& graph) {
  std::ofstream out;
  out.open(JSON_GRAPH_FILENAME, std::ofstream::out | std::ofstream::trunc);

  out << graph.to_json();

  out.close();
}

int main() {
  Graph new_graph;
  for (int i = 0; i < 14; i++) {
    new_graph.add_vertex();
  }
  new_graph.connect_vertices(0, 1);
  new_graph.connect_vertices(0, 2);
  new_graph.connect_vertices(0, 3);
  new_graph.connect_vertices(1, 4);
  new_graph.connect_vertices(1, 5);
  new_graph.connect_vertices(1, 6);
  new_graph.connect_vertices(2, 7);
  new_graph.connect_vertices(2, 8);
  new_graph.connect_vertices(3, 9);
  new_graph.connect_vertices(4, 10);
  new_graph.connect_vertices(5, 10);
  new_graph.connect_vertices(6, 10);
  new_graph.connect_vertices(7, 11);
  new_graph.connect_vertices(8, 11);
  new_graph.connect_vertices(9, 12);
  new_graph.connect_vertices(10, 13);
  new_graph.connect_vertices(11, 13);
  new_graph.connect_vertices(12, 13);

  write_graph(new_graph);

  return 0;
}
