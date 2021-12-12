#include <array>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

using std::array;
using std::string;
using std::to_string;
using std::vector;
using VertexId = int;
using EdgeId = int;
constexpr int kVerticesCount = 14;

class Graph {
 public:
  struct Vertex {
   public:
    const VertexId id;

    explicit Vertex(const VertexId& _id) : id(_id) {}

    void add_edge_id(const EdgeId& id) { edge_ids_.push_back(id); }

    const vector<EdgeId>& get_edge_ids() const { return edge_ids_; }

   private:
    vector<EdgeId> edge_ids_;
  };

  struct Edge {
    const VertexId from_vertex_id;
    const VertexId to_vertex_id;
    const EdgeId id;

    Edge(const EdgeId& _id,
         const VertexId& _from_vertex_id,
         const VertexId& _to_vertex_id)
        : id(_id),
          from_vertex_id(_from_vertex_id),
          to_vertex_id(_to_vertex_id) {}
  };

  void add_vertex() {
    const VertexId new_vertex_id = get_new_vertex_id();
    vertices_.emplace(new_vertex_id, new_vertex_id);
  }

  void add_edge(const VertexId& from_vertex_id, const VertexId& to_vertex_id) {
    const EdgeId new_edge_id = get_new_edge_id();
    edges_.emplace_back(new_edge_id, from_vertex_id, to_vertex_id);
    vertices_.at(from_vertex_id).add_edge_id(new_edge_id);
    vertices_.at(to_vertex_id).add_edge_id(new_edge_id);
  };

  const std::unordered_map<VertexId, Vertex>& get_vertices() const {
    return vertices_;
  }

  const vector<Edge>& get_edges() const { return edges_; }

 private:
  std::unordered_map<VertexId, Vertex> vertices_;
  vector<Edge> edges_;
  VertexId vertex_id_counter_ = 0;
  EdgeId edge_id_counter_ = 0;

  VertexId get_new_vertex_id() { return vertex_id_counter_++; }

  EdgeId get_new_edge_id() { return edge_id_counter_++; }
};

namespace graph_printing {
string print_vertex(const Graph::Vertex& vertex) {
  string vertex_output =
      "\"id\": " + to_string(vertex.id) + ", " + "\"edge_ids\": [";
  for (const auto& edge_id : vertex.get_edge_ids()) {
    vertex_output += to_string(edge_id);
    if (edge_id != vertex.get_edge_ids().back()) {
      vertex_output += ", ";
    }
  }
  vertex_output += "]";
  return vertex_output;
}
string print_edge(const Graph::Edge& edge) {
  return "\"id\": " + std::to_string(edge.id) + ", " + "\"vertex_ids\": [" +
         to_string(edge.from_vertex_id) + ", " + to_string(edge.to_vertex_id) +
         "]";
}
string print_graph(const Graph& graph) {
  string graph_output = "{\n\t\"vertices\": [\n\t\t";
  for (const auto& [key, vertex] : graph.get_vertices()) {
    graph_output += "{" + print_vertex(vertex) + "},\n\t\t";
  }
  graph_output = graph_output.substr(0, graph_output.size() - 4) +
                 "\n\t],\n\t\"edges\": [\n\t\t";
  for (const auto& edge : graph.get_edges()) {
    graph_output += "{" + print_edge(edge) + "}";
    if (edge.id != graph.get_edges().back().id) {
      graph_output += ",\n\t\t";
    } else {
      graph_output += "\n\t";
    }
  }
  graph_output += "]\n}\n";
  return graph_output;
}
}  // namespace graph_printing

Graph generate_graph() {
  auto graph = Graph();

  for (int i = 0; i < kVerticesCount; i++) {
    graph.add_vertex();
  }

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

  return graph;
}

void write_to_file(const string& json, const string& file_name) {
  std::ofstream file;
  file.open(file_name);
  file << json;
}

int main() {
  const auto graph = generate_graph();
  const auto graph_json = graph_printing::print_graph(graph);
  std::cout << graph_json << std::endl;
  write_to_file(graph_json, "graph.json");
}
