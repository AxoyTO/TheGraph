#include <array>
#include <fstream>
#include <iostream>
#include <vector>

using VertexId = int;

struct Vertex {
  VertexId id;
  std::vector<int> edges;
};

struct Edge {
  VertexId id;
  std::array<int, 2> vertices;
};

class Graph {
 public:
  void add_vertices(const std::vector<Vertex>& vec) { vertices_ = vec; }
  void print_vertices() const {
    std::cout << "Printing vertices:" << std::endl;
    for (int i = 0; i < vertices_.size(); ++i) {
      std::cout << i << ": [ ";
      for (int j = 0; j < vertices_[i].edges.size(); ++j)
        std::cout << vertices_[i].edges[j] << " ";
      std::cout << "]" << std::endl;
    }
  }
  void add_edges(const std::vector<Edge>& vec) { edges_ = vec; }
  void print_edges() const {
    std::cout << "Printing edges:" << std::endl;
    for (int i = 0; i < edges_.size(); ++i) {
      std::cout << i << ": [ ";
      for (int j = 0; j < edges_[i].vertices.size(); ++j)
        std::cout << edges_[i].vertices[j] << " ";
      std::cout << "]" << std::endl;
    }
  }
  std::string to_json() {
    std::string str;
    str += "{\n\t\"vertices\": [\n";
    for (int i = 0; i < vertices_.size(); ++i) {
      str += "\t\t{ \"id\": ";
      str += std::to_string(i);
      str += ", \"edge_ids\": [";
      for (int j = 0; j < vertices_[i].edges.size() - 1; ++j) {
        str += std::to_string(vertices_[i].edges[j]);
        str += ", ";
      }
      str += std::to_string(vertices_[i].edges[vertices_[i].edges.size() - 1]);
      str += "]";
      if (i + 1 < vertices_.size())
        str += " },\n";
      else
        str += " }\n";
    }
    str += "\t],\n";
    str += "\t\"edges\": [\n";
    for (int i = 0; i < edges_.size(); ++i) {
      str += "\t\t{ \"id\": ";
      str += std::to_string(i);
      str += ", \"vertex_ids\": [";
      for (int j = 0; j < edges_[i].vertices.size() - 1; ++j) {
        str += std::to_string(edges_[i].vertices[j]);
        str += ", ";
      }
      str += std::to_string(edges_[i].vertices[edges_[i].vertices.size() - 1]);
      str += "]";
      if (i + 1 < edges_.size())
        str += " },\n";
      else
        str += " }\n";
    }
    str += "\t]\n}\n";
    return str;
  }

 private:
  std::vector<Vertex> vertices_;
  std::vector<Edge> edges_;
};

void write_file(const std::string& str, std::string name) {
  std::ofstream file;
  file.open(name, std::ios::out);
  file << str;
}

int main(void) {
  Graph my_graph;
  my_graph.add_vertices((std::vector<Vertex>){{0, {0, 1, 2}},
                                              {1, {0, 3, 4, 5}},
                                              {2, {1, 6, 7}},
                                              {3, {2, 8}},
                                              {4, {3, 9}},
                                              {5, {4, 10}},
                                              {6, {5, 11}},
                                              {7, {6, 12}},
                                              {8, {7, 13}},
                                              {9, {8, 14}},
                                              {10, {9, 10, 11, 15}},
                                              {11, {12, 13, 16}},
                                              {12, {14, 17}},
                                              {13, {15, 16, 17}}});
  my_graph.add_edges((std::vector<Edge>){{0, {0, 1}},
                                         {1, {0, 2}},
                                         {2, {0, 3}},
                                         {3, {1, 4}},
                                         {4, {1, 5}},
                                         {5, {1, 6}},
                                         {6, {2, 7}},
                                         {7, {2, 8}},
                                         {8, {3, 9}},
                                         {9, {4, 10}},
                                         {10, {5, 10}},
                                         {11, {6, 10}},
                                         {12, {7, 11}},
                                         {13, {8, 11}},
                                         {14, {9, 12}},
                                         {15, {10, 13}},
                                         {16, {11, 13}},
                                         {17, {12, 13}}});
  write_file(my_graph.to_json(), "my_graph.json");
  return 0;
}
