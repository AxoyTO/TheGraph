#include <ctime>
#include <fstream>
#include <iostream>
#include <vector>

using std::cout;
using std::endl;
using std::vector;

using VertexId = int;
using EdgeId = int;

std::ostream& operator<<(std::ostream& out, const vector<int>& c) {
  if (c.size() > 0) {
    out << c[0];
    for (auto i = begin(c) + 1; i != end(c); ++i)
      out << ", " << *i;
  }
  return out;
}

struct Vertex {
  VertexId id = 0;
  int depth = 0;
  vector<EdgeId> edge_ids;
};

std::ostream& operator<<(std::ostream& out, const Vertex& c) {
  out << "{" << endl;
  out << "      \"id\": " << c.id << "," << endl;
  out << "      \"edge_ids\": [" << c.edge_ids << "]" << endl;
  out << "    }";
  return out;
}

struct Edge {
  EdgeId id = 0;
  VertexId begin = 0;
  VertexId end = 0;
  int color = 0;
};

std::ostream& operator<<(std::ostream& out, const Edge& c) {
  out << "{" << endl;
  out << "      \"id\": " << c.id << "," << endl;
  out << "      \"vertex_ids\": [" << c.begin << ", " << c.end << "]" << endl;
  out << "    }";
  return out;
}

class Graph {
 public:
  Graph task_1_graph_generation();
  friend std::ostream& operator<<(std::ostream& out, const Graph& c);

 private:
  int depth_;
  VertexId num_of_vrt_ = 0;
  EdgeId num_of_edg_ = 0;
  vector<vector<Vertex> > vertices_;
  vector<Edge> edges_;
  VertexId next_vertex_id() { return num_of_vrt_++; }
  EdgeId next_edge_id() { return num_of_edg_++; }
};

Graph Graph::task_1_graph_generation() {
  depth_ = 1;

  const VertexId num_of_vertex = 14;
  const EdgeId num_of_edges = 18;
  vertices_.resize(1);

  for (int i = 0; i < num_of_vertex; ++i) {
    Vertex x = {next_vertex_id()};
    vertices_[0].push_back(x);
  }
  vertices_[0][0].edge_ids = {0, 1, 2};
  vertices_[0][1].edge_ids = {0, 3, 4, 5};
  vertices_[0][2].edge_ids = {1, 6, 7};
  vertices_[0][3].edge_ids = {2, 8};
  vertices_[0][4].edge_ids = {3, 9};
  vertices_[0][5].edge_ids = {4, 10};
  vertices_[0][6].edge_ids = {5, 11};
  vertices_[0][7].edge_ids = {6, 12};
  vertices_[0][8].edge_ids = {7, 13};
  vertices_[0][9].edge_ids = {8, 14};
  vertices_[0][10].edge_ids = {9, 10, 11, 15};
  vertices_[0][11].edge_ids = {12, 13, 16};
  vertices_[0][12].edge_ids = {14, 17};
  vertices_[0][13].edge_ids = {15, 16, 17};
  for (int i = 0; i < num_of_edges; ++i) {
    Edge x = {next_edge_id()};
    edges_.push_back(x);
  }
  edges_[0].begin = 0;
  edges_[0].end = 1;
  edges_[1].begin = 0;
  edges_[1].end = 2;
  edges_[2].begin = 0;
  edges_[2].end = 3;
  edges_[3].begin = 1;
  edges_[3].end = 4;
  edges_[4].begin = 1;
  edges_[4].end = 5;
  edges_[5].begin = 1;
  edges_[5].end = 6;
  edges_[6].begin = 2;
  edges_[6].end = 7;
  edges_[7].begin = 2;
  edges_[7].end = 8;
  edges_[8].begin = 3;
  edges_[8].end = 9;
  edges_[9].begin = 4;
  edges_[9].end = 10;
  edges_[10].begin = 5;
  edges_[10].end = 10;
  edges_[11].begin = 6;
  edges_[11].end = 10;
  edges_[12].begin = 7;
  edges_[12].end = 11;
  edges_[13].begin = 8;
  edges_[13].end = 11;
  edges_[14].begin = 9;
  edges_[14].end = 12;
  edges_[15].begin = 10;
  edges_[15].end = 13;
  edges_[16].begin = 11;
  edges_[16].end = 13;
  edges_[17].begin = 12;
  edges_[17].end = 13;

  return *this;
}

std::ostream& operator<<(std::ostream& out, const vector<Vertex>& c) {
  if (c.size() > 0) {
    out << c[0];
    for (auto i = begin(c) + 1; i != end(c); ++i)
      out << ", " << *i;
  }
  return out;
}

std::ostream& operator<<(std::ostream& out, const vector<vector<Vertex> >& c) {
  if (c.size() > 0) {
    out << c[0];
    for (auto i = begin(c) + 1; i != end(c); ++i)
      out << ", " << *i;
  }
  return out;
}

std::ostream& operator<<(std::ostream& out, const vector<Edge>& c) {
  if (c.size() > 0) {
    out << c[0];
    for (auto i = begin(c) + 1; i != end(c); ++i)
      out << ", " << *i;
  }
  return out;
}

std::ostream& operator<<(std::ostream& out, const Graph& c) {
  out << "{" << std::endl;
  out << "  \"vertices\": [" << endl
      << "   " << c.vertices_ << endl
      << "  ]," << endl;
  out << "  \"edges\": [" << endl
      << "    " << c.edges_ << endl
      << "  ]" << endl
      << "}" << endl;
  return out;
}

int main() {
  Graph A = Graph().task_1_graph_generation();
  std::ofstream out("graph_task_02.json");
  out << A;
  return 0;
}
