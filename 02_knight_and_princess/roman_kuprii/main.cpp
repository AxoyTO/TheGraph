
#include <array>
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

#define MIN(a, b) ((a) < (b) ? (a) : (b))

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

struct Vertex {
  const VertexId id = INVALID_ID;
  std::array<EdgeId, 2> edges_ids;

  explicit Vertex(VertexId _id) : id(_id) {}

  std::string to_json() const {
    std::string res;
    res = "{ \"id\": ";
    res += to_string(id) + ", \"edge_ids\": [";
    for (int n : edges_ids) {
      res += to_string(n);
      res += ", ";
    }
    res.pop_back();
    res.pop_back();
    res += "] }";
    return res;
  }
};

class Graph {
 public:
  Graph() {}

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

  void add_vertex() {
    Vertex new_vertex(vertices_.size());
    vertices_.emplace_back(new_vertex);
  }

  void connect_vertices(VertexId out_id, VertexId dest_id) {
    EdgeId id = edges_.size();
    Edge new_edge(out_id, dest_id, id);
    edges_.push_back(new_edge);

    // add information into Verex structure
    vertices_[out_id].edges_ids[0] = id;
    vertices_[dest_id].edges_ids[1] = id;
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
