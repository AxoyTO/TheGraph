#include <array>
#include <fstream>
#include <iostream>
#include <vector>

using VertexId = int;
using EdgeId = int;

struct Vertex {
  VertexId id;
  std::vector<int> edges;

  std::string& to_json() {
    static std::string string;
    string.clear();
    string += "{ \"id\": ";
    string += std::to_string(id);
    string += ", \"edge_ids\": [";
    for (auto it = edges.begin(); it != edges.end() - 1; it++) {
      string += std::to_string(*it);
      string += ", ";
    }
    string += std::to_string(*(edges.end() - 1));
    string += "] }";
    return string;
  }
};

struct Edge {
  EdgeId id;
  std::array<int, 2> vertices;

  std::string& to_json() {
    static std::string string;
    string.clear();
    string += "{ \"id\": ";
    string += std::to_string(id);
    string += ", \"vertex_ids\": [";
    string += std::to_string(vertices[0]);
    string += ", ";
    string += std::to_string(vertices[1]);
    string += "] }";
    return string;
  }
};

class Graph {
 public:
  Graph(const std::vector<Vertex>& vertices, const std::vector<Edge>& edges)
      : vertices_(vertices), edges_(edges) {}

  void print_vertices() const {
    std::cout << "Printing vertices:" << std::endl;
    for (const auto& vertex : vertices_) {
      std::cout << vertex.id << ": [ ";
      for (const auto& edge : vertex.edges)
        std::cout << edge << " ";
      std::cout << "]" << std::endl;
    }
  }

  void print_edges() const {
    std::cout << "Printing edges:" << std::endl;
    for (const auto& edge : edges_) {
      std::cout << edge.id << ": [ ";
      for (const auto& vertex : edge.vertices)
        std::cout << vertex << " ";
      std::cout << "]" << std::endl;
    }
  }

  std::string to_json() {
    std::string str;
    str += "{\n\t\"vertices\": [\n";
    for (auto it = vertices_.begin(); it != vertices_.end(); it++) {
      str += "\t\t";
      str += it->to_json();
      if (it != vertices_.end() - 1)
        str += ",";
      str += "\n";
    }
    str += "\t],\n";
    str += "\t\"edges\": [\n";
    for (auto it = edges_.begin(); it != edges_.end(); it++) {
      str += "\t\t";
      str += it->to_json();
      if (it != edges_.end() - 1)
        str += ",";
      str += "\n";
    }
    str += "\t]\n}\n";
    return str;
  }

 private:
  std::vector<Vertex> vertices_;
  std::vector<Edge> edges_;
};

void write_file(const std::string& string, const std::string& name) {
  std::ofstream file;
  file.open(name, std::ios::out);
  file << string;
}

int main(void) {
  const std::vector<Vertex> vertices = {
      {0, {0, 1, 2}}, {1, {0, 3, 4, 5}},     {2, {1, 6, 7}},
      {3, {2, 8}},    {4, {3, 9}},           {5, {4, 10}},
      {6, {5, 11}},   {7, {6, 12}},          {8, {7, 13}},
      {9, {8, 14}},   {10, {9, 10, 11, 15}}, {11, {12, 13, 16}},
      {12, {14, 17}}, {13, {15, 16, 17}}};
  const std::vector<Edge> edges = {
      {0, {0, 1}},    {1, {0, 2}},   {2, {0, 3}},   {3, {1, 4}},
      {4, {1, 5}},    {5, {1, 6}},   {6, {2, 7}},   {7, {2, 8}},
      {8, {3, 9}},    {9, {4, 10}},  {10, {5, 10}}, {11, {6, 10}},
      {12, {7, 11}},  {13, {8, 11}}, {14, {9, 12}}, {15, {10, 13}},
      {16, {11, 13}}, {17, {12, 13}}};
  auto my_graph = Graph(vertices, edges);
  write_file(my_graph.to_json(), "my_graph.json");
  return 0;
}
