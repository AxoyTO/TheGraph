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
  VertexId id = 0;
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
  EdgeId id = 0;
  VertexId begin = 0;
  VertexId end = 0;
};

std::ostream& operator<<(std::ostream& out, const Edge& edge) {
  out << "{" << endl;
  out << "      \"id\": " << edge.id << "," << endl;
  out << "      \"vertex_ids\": [" << edge.begin << ", " << edge.end << "]" << endl;
  out << "    }";
  return out;
}

class Graph {
 public:

  void resize(int graph_depth) { vertices_.resize(graph_depth); }
  VertexId next_vertex_id() { return num_of_vrt_++; }
  EdgeId next_edge_id() { return num_of_edg_++; }
  void add_vertex(Vertex& vertex, int layer); 
  void add_edge(Edge& edge) { edges_.push_back(edge); }
  void add_edge_in_vertex(int layer, int vertex_in_layer, EdgeId edge_id);
  bool layer_is_empty_or_not_exist(int layer);
  VertexId get_vertex_id(int layer, int vertex_in_layer);
  size_t layer_size(int layer);
  
  const vector<vector<Vertex>> vertices() const { return vertices_; }
  const vector<Edge> edges() const { return edges_; }  

 private:
  int depth_ = 0;
  VertexId num_of_vrt_ = 0;
  EdgeId num_of_edg_ = 0;
  vector<vector<Vertex> > vertices_;
  vector<Edge> edges_;
};

void Graph::add_vertex(Vertex& vertex, int layer) {
    if (layer >= depth_) {
        depth_ = layer + 1;
    }
    vertices_[layer].push_back(vertex);
}

void Graph::add_edge_in_vertex(int layer, int vertex_in_layer, EdgeId edge_id) {
    vertices_[layer][vertex_in_layer].edge_ids.push_back(edge_id);
}

bool Graph::layer_is_empty_or_not_exist(int layer) {
  return (layer >= depth_) || vertices_[layer].empty();
}

VertexId Graph::get_vertex_id(int layer, int vertex_in_layer) {
    return vertices_[layer][vertex_in_layer].id;
}

size_t Graph::layer_size(int layer) {
    if (layer_is_empty_or_not_exist(layer)) {
        return 0;
    }
    return vertices_[layer].size();
}

Graph task_1_graph_generation() {
  Graph returned_graph;
  const VertexId num_of_vertex = 14;
  returned_graph.resize(1);
  
  for (int i = 0; i < num_of_vertex; ++i) {
    Vertex x = {returned_graph.next_vertex_id()};
    returned_graph.add_vertex(x, 0);
  }
  returned_graph.add_edge_in_vertex(0, 0, 0);
  returned_graph.add_edge_in_vertex(0, 0, 1);
  returned_graph.add_edge_in_vertex(0, 0, 2);
  returned_graph.add_edge_in_vertex(0, 1, 0);
  returned_graph.add_edge_in_vertex(0, 1, 3);
  returned_graph.add_edge_in_vertex(0, 1, 4);
  returned_graph.add_edge_in_vertex(0, 1, 5);
  returned_graph.add_edge_in_vertex(0, 2, 1);  
  returned_graph.add_edge_in_vertex(0, 2, 6);  
  returned_graph.add_edge_in_vertex(0, 2, 7);
  returned_graph.add_edge_in_vertex(0, 3, 2);
  returned_graph.add_edge_in_vertex(0, 3, 8);
  returned_graph.add_edge_in_vertex(0, 4, 3);
  returned_graph.add_edge_in_vertex(0, 4, 9);
  returned_graph.add_edge_in_vertex(0, 5, 4);
  returned_graph.add_edge_in_vertex(0, 5, 10);
  returned_graph.add_edge_in_vertex(0, 6, 5);
  returned_graph.add_edge_in_vertex(0, 6, 11);
  returned_graph.add_edge_in_vertex(0, 7, 6);
  returned_graph.add_edge_in_vertex(0, 7, 12);
  returned_graph.add_edge_in_vertex(0, 8, 7);
  returned_graph.add_edge_in_vertex(0, 8, 13);
  returned_graph.add_edge_in_vertex(0, 9, 8);
  returned_graph.add_edge_in_vertex(0, 9, 14);
  returned_graph.add_edge_in_vertex(0, 10, 9);
  returned_graph.add_edge_in_vertex(0, 10, 10);
  returned_graph.add_edge_in_vertex(0, 10, 11);
  returned_graph.add_edge_in_vertex(0, 10, 15);
  returned_graph.add_edge_in_vertex(0, 11, 12);
  returned_graph.add_edge_in_vertex(0, 11, 13);
  returned_graph.add_edge_in_vertex(0, 11, 16);
  returned_graph.add_edge_in_vertex(0, 12, 14);
  returned_graph.add_edge_in_vertex(0, 12, 17);
  returned_graph.add_edge_in_vertex(0, 13, 15);
  returned_graph.add_edge_in_vertex(0, 13, 16);
  returned_graph.add_edge_in_vertex(0, 13, 17);
  Edge tmp = {returned_graph.next_edge_id(), 0, 1};
  returned_graph.add_edge(tmp);
  Edge tmp1 = {returned_graph.next_edge_id(), 0, 2};
  returned_graph.add_edge(tmp1);
  Edge tmp100 = {returned_graph.next_edge_id(), 0, 3};
  returned_graph.add_edge(tmp100);
  Edge tmp2 = {returned_graph.next_edge_id(), 1, 4};
  returned_graph.add_edge(tmp2);
  Edge tmp3 = {returned_graph.next_edge_id(), 1, 5};
  returned_graph.add_edge(tmp3);
  Edge tmp4 = {returned_graph.next_edge_id(), 1, 6};
  returned_graph.add_edge(tmp4);
  Edge tmp5 = {returned_graph.next_edge_id(), 2, 7};
  returned_graph.add_edge(tmp5);
  Edge tmp6 = {returned_graph.next_edge_id(), 2, 8};
  returned_graph.add_edge(tmp6);
  Edge tmp7 = {returned_graph.next_edge_id(), 3, 9};
  returned_graph.add_edge(tmp7);
  Edge tmp8 = {returned_graph.next_edge_id(), 4, 10};
  returned_graph.add_edge(tmp8);
  Edge tmp9 = {returned_graph.next_edge_id(), 5, 10};
  returned_graph.add_edge(tmp9);
  Edge tmp10 = {returned_graph.next_edge_id(), 6, 10};
  returned_graph.add_edge(tmp10);
  Edge tmp11 = {returned_graph.next_edge_id(), 7, 11};
  returned_graph.add_edge(tmp11);
  Edge tmp12 = {returned_graph.next_edge_id(), 8, 11};
  returned_graph.add_edge(tmp12);
  Edge tmp13 = {returned_graph.next_edge_id(), 9, 12};
  returned_graph.add_edge(tmp13);
  Edge tmp14 = {returned_graph.next_edge_id(), 10, 13};
  returned_graph.add_edge(tmp14);
  Edge tmp15 = {returned_graph.next_edge_id(), 11, 13};
  returned_graph.add_edge(tmp15);
  Edge tmp16 = {returned_graph.next_edge_id(), 12, 13};
  returned_graph.add_edge(tmp16);

  return returned_graph;
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

std::ostream& operator<<(std::ostream& out, const vector<vector<Vertex>>& vertices) {
  for (auto it = vertices.begin(); it != vertices.end(); ++it) {
    out << *it;
    if (it != vertices.end() - 1) {
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

int main() {
  const Graph graph = task_1_graph_generation();
  std::ofstream out("graph_task_02.json");
  out << graph;
  out.close();
  return 0;
}
