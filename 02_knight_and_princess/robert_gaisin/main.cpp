#include <ctime>
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
  const VertexId id = 0;
  int depth = 0;
  vector<EdgeId> edge_ids;
};

std::ostream& operator<<(std::ostream& out, const Vertex& vertex) {
  out << "{" << endl;
  out << "      \"id\": " << vertex.id << "," << endl;
  out << "      \"edge_ids\": [" << vertex.edge_ids << "]" << endl;
  out << "}";
  return out;
}

struct Edge {
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

 private:
  VertexId num_of_vrt_ = 0;
  EdgeId num_of_edg_ = 0;

  VertexId next_vertex_id() { return num_of_vrt_++; }
  EdgeId next_edge_id() { return num_of_edg_++; }

  bool connection(const VertexId& begin, const VertexId& end);
  vector<Vertex> vertices_;
  vector<Edge> edges_;
};

bool Graph::connection(const VertexId& begin, const VertexId& end) {
  bool ret = false;
  for (const EdgeId& edge_num : vertices_[begin].edge_ids) {
    if ((ret = edges_[edge_num].begin == end || edges_[edge_num].end == end)) {
      break;
    }
  }
  return ret;
}

void Graph::add_vertex() {
  Vertex vertex = {next_vertex_id()};
  vertices_.push_back(vertex);
}

void Graph::add_edge(const VertexId& begin, const VertexId& end) {
  if ((begin < num_of_vrt_) && (end < num_of_vrt_) && !connection(begin, end)) {
    Edge edge = {next_edge_id(), begin, end};
    vertices_[begin].edge_ids.push_back(edge.id);
    vertices_[end].edge_ids.push_back(edge.id);
    edges_.push_back(edge);
  }
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
  const VertexId num_of_vertex = 14;

  for (int i = 0; i < num_of_vertex; ++i) {
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
