#include <array>
#include <cassert>
#include <fstream>
#include <iostream>
#include <set>
#include <sstream>
#include <string>
#include <unordered_map>
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
  Vertex& add_vertex() {
    VertexId new_vertex_id = get_new_vertex_id();
    adjacency_list_[new_vertex_id] = std::set<EdgeId>();
    return vertices_.emplace_back(new_vertex_id);
  };

  bool has_vertex(const VertexId& vertex_id) const {
    for (auto vertex : vertices_) {
      if (vertex.id == vertex_id) {
        return true;
      }
    }
    return false;
  };

  Edge& add_edge(const VertexId& from_vertex_id, const VertexId& to_vertex_id) {
    assert(has_vertex(from_vertex_id));
    assert(has_vertex(to_vertex_id));
    EdgeId new_edge_id = get_new_edge_id();
    adjacency_list_[from_vertex_id].insert(new_edge_id);
    adjacency_list_[to_vertex_id].insert(new_edge_id);
    return edges_.emplace_back(new_edge_id, from_vertex_id, to_vertex_id);
  };

  bool has_edge(const EdgeId& edge_id) const {
    for (auto edge : edges_) {
      if (edge.id == edge_id) {
        return true;
      }
    }
    return false;
  };

  const Vertex& get_vertex(const VertexId& vertex_id) const {
    for (const auto& vertex : vertices_) {
      if (vertex.id == vertex_id) {
        return vertex;
      }
    }
    throw std::runtime_error("No such vertex");
  }
  const Edge& get_edge(const EdgeId& edge_id) const {
    for (const auto& edge : edges_) {
      if (edge.id == edge_id) {
        return edge;
      }
    }
    throw std::runtime_error("No such edge");
  }

  const std::vector<Vertex>& vertices() const { return vertices_; }
  const std::vector<Edge>& edges() const { return edges_; }

  const std::set<EdgeId>& connected_edge_ids(const VertexId& id) const {
    return adjacency_list_.at(id);
  }

 private:
  std::vector<Edge> edges_;
  std::vector<Vertex> vertices_;
  std::unordered_map<VertexId, std::set<EdgeId>> adjacency_list_;

  EdgeId edges_id_counter_ = 0;
  VertexId vertex_id_counter_ = 0;

  VertexId get_new_vertex_id() { return vertex_id_counter_++; }
  EdgeId get_new_edge_id() { return edges_id_counter_++; }
};

class GraphPrinter {
 public:
  explicit GraphPrinter(const Graph& graph) : graph_(graph) {}

  std::string print_vertex(const Vertex& vertex) const {
    std::stringstream result_stream;
    result_stream << "\t\t{" << std::endl;
    result_stream << "\t\t\t\"id\": " << vertex.id << "," << std::endl;
    result_stream << "\t\t\t\"edge_ids\": [";

    const auto& edge_ids = graph_.connected_edge_ids(vertex.id);

    for (const auto& edge_id : edge_ids) {
      result_stream << edge_id;
      if (edge_id != *(edge_ids.rbegin())) {
        result_stream << ", ";
      }
    }

    result_stream << "]" << std::endl;
    result_stream << "\t\t}";

    return result_stream.str();
  }

  std::string print_edge(const Edge& edge) const {
    std::stringstream result_stream;
    result_stream << "\t\t{" << std::endl;
    result_stream << "\t\t\t\"id\": " << edge.id << "," << std::endl;
    result_stream << "\t\t\t\"vertex_ids\": [";
    result_stream << edge.from_vertex_id << ", " << edge.to_vertex_id;
    result_stream << "]" << std::endl;
    result_stream << "\t\t}";

    return result_stream.str();
  }

  std::string print() const {
    std::stringstream result_stream;
    std::cout << "Printing graph to string" << std::endl;

    result_stream << "{" << std::endl;
    std::cout << "\tPrinting vertices to string" << std::endl;

    result_stream << "\t\"vertices\": [" << std::endl;

    const auto& vertices = graph_.vertices();
    for (const auto& vertex : vertices) {
      result_stream << print_vertex(vertex);
      if (vertex.id != vertices.back().id) {
        result_stream << ",";
      }
      result_stream << std::endl;
    }

    result_stream << std::endl
                  << "\t]"
                  << "," << std::endl;

    const auto& edges = graph_.edges();
    std::cout << "\tPrinting edges to string" << std::endl;
    result_stream << "\t\"edges\": [" << std::endl;
    for (const auto& edge : edges) {
      result_stream << print_edge(edge);
      if (edge.id != edges.back().id) {
        result_stream << ",";
      }
      result_stream << std::endl;
    }
    result_stream << "\t]" << std::endl << "}";
    return result_stream.str();
  }

 private:
  const Graph& graph_;
};

void string_to_file(const std::string& graph_json,
                    const std::string& file_path) {
  std::ofstream out_file;
  out_file.open(file_path);
  out_file << graph_json;
  out_file.close();
}

constexpr int kVerticesCount = 14;

Graph generate_graph() {
  std::cout << "Graph generating:" << std::endl;

  auto graph = Graph();

  for (int i = 0; i < kVerticesCount; i++) {
    graph.add_vertex();
  }

  std::cout << "\tVerices were added" << std::endl;

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

  std::cout << "\tEdges were added" << std::endl;

  return (graph);
}

int main(int argc, char* argv[]) {
  const auto graph = generate_graph();

  const auto graph_printer = GraphPrinter(graph);
  const auto graph_json = graph_printer.print();

  std::cout << "Graph converted to json string" << std::endl;

  std::cout << graph_json << std::endl;

  std::string out_file("graph.json");

  if (argc > 1) {
    out_file = std::string(argv[1]);
  }
  string_to_file(graph_json, out_file);

  std::cout << out_file << " file has been created (if not existed) and filled"
            << std::endl;
  std::cout << "Have a nice day!" << std::endl;

  return 0;
}
