#include <cassert>
#include <fstream>
#include <iostream>
#include <vector>

using std::cout;
using std::endl;
using std::vector;

using VertexId = int;
using EdgeId = int;

std::ostream& operator<<(std::ostream& out, const vector<int>& int_vector) {
  for (auto it = int_vector.begin(); it != int_vector.end(); ++it) {
    out << *it;
    if (it != int_vector.end() - 1) {
      out << ", ";
    }
  }
  return out;
}

struct Vertex {
  explicit Vertex(const VertexId& new_id) : id(new_id) {}
  const VertexId id = 0;
  void add_edge_id(const EdgeId& id);
  const vector<EdgeId>& edge_ids() const { return edge_ids_; }
  bool has_edge_id(const EdgeId& id) const;

 private:
  vector<EdgeId> edge_ids_;
};

bool Vertex::has_edge_id(const EdgeId& id) const {
  for (const auto& edge_id : edge_ids_)
    if (id == edge_id)
      return true;
  return false;
}

void Vertex::add_edge_id(const EdgeId& id) {
  assert(!has_edge_id(id) && "Edge doesn't exist");
  edge_ids_.push_back(id);
}

std::ostream& operator<<(std::ostream& out, const Vertex& vertex) {
  out << "{" << endl;
  out << "      \"id\": " << vertex.id << "," << endl;
  out << "      \"edge_ids\": [" << vertex.edge_ids() << "]" << endl;
  out << "}";
  return out;
}

struct Edge {
  Edge(const EdgeId& new_id,
       const VertexId& begin_vertex,
       const VertexId& end_vertex)
      : id(new_id), begin(begin_vertex), end(end_vertex) {}
  const EdgeId id = 0;
  const VertexId begin = 0;
  const VertexId end = 0;
};

std::ostream& operator<<(std::ostream& out, const Edge& edge) {
  out << "{" << endl;
  out << "      \"id\": " << edge.id << "," << endl;
  out << "      \"vertex_ids\": [" << edge.begin << ", " << edge.end << "]"
      << endl;
  out << "}";
  return out;
}

class Graph {
 public:
  void add_vertex();

  void add_edge(const VertexId& begin, const VertexId& end);

  const vector<Vertex>& vertices() const { return vertices_; }
  const vector<Edge>& edges() const { return edges_; }

  bool has_vertex(const VertexId& vertex_id) const;
  bool is_connected(const VertexId& begin, const VertexId& end) const;

 private:
  VertexId num_of_vrt_ = 0;
  EdgeId num_of_edg_ = 0;

  VertexId next_vertex_id() { return num_of_vrt_++; }
  EdgeId next_edge_id() { return num_of_edg_++; }

  vector<Vertex> vertices_;
  vector<Edge> edges_;
};

bool Graph::has_vertex(const VertexId& vertex_id) const {
  for (const auto& vertex : vertices_) {
    if (vertex_id == vertex.id)
      return true;
  }
  return false;
}

bool Graph::is_connected(const VertexId& begin, const VertexId& end) const {
  assert(has_vertex(begin) && "Vertex doesn't exist");
  assert(has_vertex(end) && "Vertex doesn't exist");
  for (const EdgeId& edge_num : vertices_[begin].edge_ids()) {
    if (edges_[edge_num].begin == end || edges_[edge_num].end == end) {
      return true;
    }
  }
  return false;
}

void Graph::add_vertex() {
  vertices_.emplace_back(next_vertex_id());
}

void Graph::add_edge(const VertexId& begin, const VertexId& end) {
  assert(has_vertex(begin) && "Vertex doesn't exist");
  assert(has_vertex(end) && "Vertex doesn't exist");
  assert(!is_connected(begin, end) && "Vertices already connected");

  const auto& edge = edges_.emplace_back(next_edge_id(), begin, end);
  vertices_[begin].add_edge_id(edge.id);
  vertices_[end].add_edge_id(edge.id);
}

std::ostream& operator<<(std::ostream& out, const vector<Vertex>& layer) {
  for (auto it = layer.begin(); it != layer.end(); ++it) {
    out << *it;
    if (it != layer.end() - 1) {
      out << ", ";
    }
  }
  return out;
}

std::ostream& operator<<(std::ostream& out, const vector<Edge>& edges) {
  for (auto it = edges.begin(); it != edges.end(); ++it) {
    out << *it;
    if (it != edges.end() - 1) {
      out << ", ";
    }
  }
  return out;
}

std::ostream& operator<<(std::ostream& out, const Graph& graph) {
  out << "{" << std::endl;
  out << "  \"vertices\": [" << endl
      << "   " << graph.vertices() << endl
      << "  ]," << endl;
  out << "  \"edges\": [" << endl
      << "    " << graph.edges() << endl
      << "  ]" << endl
      << "}" << endl;
  return out;
}

Graph task_1_graph_generation() {
  auto returned_graph = Graph();
  const VertexId num_of_vertices = 14;

  for (int i = 0; i < num_of_vertices; ++i) {
    returned_graph.add_vertex();
  }

  returned_graph.add_edge(0, 1);
  returned_graph.add_edge(0, 2);
  returned_graph.add_edge(0, 3);
  returned_graph.add_edge(1, 4);
  returned_graph.add_edge(1, 5);
  returned_graph.add_edge(1, 6);
  returned_graph.add_edge(2, 7);
  returned_graph.add_edge(2, 8);
  returned_graph.add_edge(3, 9);
  returned_graph.add_edge(4, 10);
  returned_graph.add_edge(5, 10);
  returned_graph.add_edge(6, 10);
  returned_graph.add_edge(7, 11);
  returned_graph.add_edge(8, 11);
  returned_graph.add_edge(9, 12);
  returned_graph.add_edge(10, 13);
  returned_graph.add_edge(11, 13);
  returned_graph.add_edge(12, 13);

  return returned_graph;
}

int main() {
  const Graph graph = task_1_graph_generation();
  std::ofstream out("graph_task_02.json");
  out << graph;
  out.close();
  return 0;
}
