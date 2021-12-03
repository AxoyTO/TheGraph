#include <array>
#include <cassert>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using VertexId = int;
using EdgeId = int;

struct Vertex {
  explicit Vertex(const VertexId& _id) : id(_id){};
  const VertexId id = 0;
};

struct Edge {
  Edge(const EdgeId& _id,
       const VertexId& _from_vertex_id,
       const VertexId& _to_vertex_id)
      : id(_id), from_vertex_id(_from_vertex_id), to_vertex_id(_to_vertex_id) {}

  const EdgeId id = 0;
  const VertexId from_vertex_id = 0;
  const VertexId to_vertex_id = 0;
};

class Graph {
 public:
  Vertex& add_vertex() { return vertices_.emplace_back(get_new_vertex_id()); };

  bool has_vertex(const VertexId& vertex_id) const {
    for (auto ver_i = vertices_.cbegin(); ver_i != vertices_.cend(); ++ver_i) {
      if (ver_i->id == vertex_id) {
        return true;
      }
    }
    return false;
  };

  bool has_edge(const EdgeId& edge_id) const {
    bool exist = false;
    for (auto edg_i = edges_.cbegin(); edg_i != edges_.cend(); ++edg_i) {
      if (edg_i->id == edge_id) {
        exist = true;
      }
    }
    return exist;
  };

  Edge& add_edge(const VertexId& from_vertex_id, const VertexId& to_vertex_id) {
    assert(has_vertex(from_vertex_id));
    assert(has_vertex(to_vertex_id));
    return edges_.emplace_back(get_new_edge_id(), from_vertex_id, to_vertex_id);
  };

  const Vertex& get_vertex(const VertexId& vertex_id) const {
    for (auto ver_i = vertices_.cbegin(); ver_i != vertices_.cend(); ++ver_i) {
      if (ver_i->id == vertex_id) {
        return (*ver_i);
      }
    }
    // return nullptr;
  }
  const Edge& get_edge(const EdgeId& edge_id) const {
    for (auto edg_i = edges_.cbegin(); edg_i != edges_.cend(); ++edg_i) {
      if (edg_i->id == edge_id) {
        return (*edg_i);
      }
    }
    // return nullptr;
  }
  void get_vertexes_edges(const Vertex& ver, std::vector<Edge>& edges) const {
    for (auto edg_i = edges_.cbegin(); edg_i != edges_.cend(); ++edg_i) {
      if ((edg_i->from_vertex_id == ver.id) ||
          (edg_i->to_vertex_id == ver.id)) {
        edges.push_back(*edg_i);
      }
    }
  }

 private:
  std::vector<Edge> edges_;
  std::vector<Vertex> vertices_;
  int edges_num_ = 0;
  int vertices_num_ = 0;

  int get_new_vertex_id() { return vertices_num_++; }
  int get_new_edge_id() { return edges_num_++; }
};

class GraphPrinter {
 public:
  explicit GraphPrinter(const Graph& graph) : graph_(graph) {}

  std::string print_vertex(const Vertex& ver) const {
    std::stringstream ss;
    std::vector<Edge> edges;
    graph_.get_vertexes_edges(ver, edges);
    ss << "\t\t{" << std::endl;
    ss << "\t\t\t\"id\": " << ver.id << "," << std::endl;
    ss << "\t\t\t\"edge_ids\": [";
    int edges_count = 0;
    for (auto edg_i = edges.cbegin(); edg_i < edges.cend(); ++edg_i) {
      if ((edg_i->from_vertex_id == ver.id) ||
          (edg_i->to_vertex_id == ver.id)) {
        if (edges_count++) {
          ss << ", ";
        }
        ss << edg_i->id;
      }
    }
    ss << "]" << std::endl;
    ss << "\t\t}";

    return ss.str();
  }

  std::string print_edge(const Edge& edg) const {
    std::stringstream ss;
    ss << "\t\t{" << std::endl;
    ss << "\t\t\t\"id\": " << edg.id << "," << std::endl;
    ss << "\t\t\t\"vertex_ids\": [";
    ss << edg.from_vertex_id << ", " << edg.to_vertex_id;
    ss << "]" << std::endl;
    ss << "\t\t}";

    return ss.str();
  }

  std::string print() const {
    std::stringstream ss;
    std::cout << "Printing graph to string" << std::endl;

    ss << "{" << std::endl;
    std::cout << "\tPrinting vertices to string" << std::endl;

    ss << "\t\"vertices\": [" << std::endl;
    int i = 0;
    while (graph_.has_vertex(i)) {
      if (i++) {
        ss << "," << std::endl;
      }
      ss << print_vertex(graph_.get_vertex(i - 1));
    }

    ss << std::endl
       << "\t]"
       << "," << std::endl;

    i = 0;

    std::cout << "\tPrinting edges to string" << std::endl;
    ss << "\t\"edges\": [" << std::endl;
    while (graph_.has_edge(i)) {
      if (i++) {
        ss << "," << std::endl;
      }
      ss << print_edge(graph_.get_edge(i - 1));
    }
    ss << std::endl << "\t]" << std::endl << "}";
    return ss.str();
  }

 private:
  const Graph& graph_;
};

void str_to_file(std::string graph_json, const char* file_path) {
  std::ofstream out_file;
  out_file.open(file_path);
  out_file << graph_json;
  out_file.close();
}

constexpr int kVerticesCount = 14;

int main(int argc, char* argv[]) {
  auto graph = Graph();

  for (int i = 0; i < kVerticesCount; i++) {
    graph.add_vertex();
  }

  std::cout << "Verices were added" << std::endl;

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
  // graph.add_edge(33, 41);

  std::cout << "Edges were added" << std::endl;
  const auto graph_printer = GraphPrinter(graph);
  const auto graph_json = graph_printer.print();

  std::cout << "Graph converted to json string" << std::endl;

  std::cout << graph_json << std::endl;
  str_to_file(graph_json, "graph.json");

  std::cout << argv[1] << " file has been created (if not existed) and filled"
            << std::endl;
  std::cout << "Have a nice day!" << std::endl;

  return 0;
}