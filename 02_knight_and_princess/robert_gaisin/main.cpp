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
  const VertexId id;
  int depth = 0;
  vector<EdgeId> edge_ids;
};

std::ostream& operator<<(std::ostream& out, const Vertex& vertex) {
  out << "{" << endl;
  out << "      \"id\": " << vertex.id << "," << endl;
  out << "      \"edge_ids\": [" << vertex.edge_ids << "]" << endl;
  out << "    }";
  return out;
}

struct Edge {
  const EdgeId id;
  VertexId begin = 0;
  VertexId end = 0;
};

std::ostream& operator<<(std::ostream& out, const Edge& edge) {
  out << "{" << endl;
  out << "      \"id\": " << edge.id << "," << endl;
  out << "      \"vertex_ids\": [" << edge.begin << ", " << edge.end << "]"
      << endl;
  out << "    }";
  return out;
}

class Graph {
 public:
  void add_vertex();

  void add_edge(VertexId begin, VertexId end);

  void add_edge_in_vertex(VertexId vertex_id, EdgeId edge_id);

  const vector<Vertex>& vertices() const { return vertices_; }
  const vector<Edge>& edges() const { return edges_; }

 private:
  VertexId num_of_vrt_ = 0;
  EdgeId num_of_edg_ = 0;

  VertexId next_vertex_id() { return num_of_vrt_++; }
  EdgeId next_edge_id() { return num_of_edg_++; }

  vector<Vertex> vertices_;
  vector<Edge> edges_;
};

void Graph::add_vertex() {
  Vertex vertex = {next_vertex_id()};
  vertices_.push_back(vertex);
}

void Graph::add_edge(VertexId begin, VertexId end) {
  Edge edge = {next_edge_id(), begin, end};
  edges_.push_back(edge);
}

void Graph::add_edge_in_vertex(VertexId vertex_id, EdgeId edge_id) {
  if (vertex_id <= num_of_vrt_)
    vertices_[vertex_id].edge_ids.push_back(edge_id);
  else
    cout << "Аttempt to create an edge with a non-existent vertex" << endl;
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

  returned_graph.add_edge_in_vertex(0, 0);
  returned_graph.add_edge_in_vertex(0, 1);
  returned_graph.add_edge_in_vertex(0, 2);
  returned_graph.add_edge_in_vertex(1, 0);
  returned_graph.add_edge_in_vertex(1, 3);
  returned_graph.add_edge_in_vertex(1, 4);
  returned_graph.add_edge_in_vertex(1, 5);
  returned_graph.add_edge_in_vertex(2, 1);
  returned_graph.add_edge_in_vertex(2, 6);
  returned_graph.add_edge_in_vertex(2, 7);
  returned_graph.add_edge_in_vertex(3, 2);
  returned_graph.add_edge_in_vertex(3, 8);
  returned_graph.add_edge_in_vertex(4, 3);
  returned_graph.add_edge_in_vertex(4, 9);
  returned_graph.add_edge_in_vertex(5, 4);
  returned_graph.add_edge_in_vertex(5, 10);
  returned_graph.add_edge_in_vertex(6, 5);
  returned_graph.add_edge_in_vertex(6, 11);
  returned_graph.add_edge_in_vertex(7, 6);
  returned_graph.add_edge_in_vertex(7, 12);
  returned_graph.add_edge_in_vertex(8, 7);
  returned_graph.add_edge_in_vertex(8, 13);
  returned_graph.add_edge_in_vertex(9, 8);
  returned_graph.add_edge_in_vertex(9, 14);
  returned_graph.add_edge_in_vertex(10, 9);
  returned_graph.add_edge_in_vertex(10, 10);
  returned_graph.add_edge_in_vertex(10, 11);
  returned_graph.add_edge_in_vertex(10, 15);
  returned_graph.add_edge_in_vertex(11, 12);
  returned_graph.add_edge_in_vertex(11, 13);
  returned_graph.add_edge_in_vertex(11, 16);
  returned_graph.add_edge_in_vertex(12, 14);
  returned_graph.add_edge_in_vertex(12, 17);
  returned_graph.add_edge_in_vertex(13, 15);
  returned_graph.add_edge_in_vertex(13, 16);
  returned_graph.add_edge_in_vertex(13, 17);

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
