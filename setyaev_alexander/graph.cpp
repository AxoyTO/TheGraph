#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using VertexId = int;
using EdgeId = int;

constexpr int VERTICES_COUNT = 14;

class Vertex {
 public:
  explicit Vertex(VertexId id) : id_(id) {}
  VertexId get_id() const;
  const std::vector<EdgeId>& get_connected_edge_ids() const;
  void add_connected_edge_id(EdgeId id);
  std::string to_string() const;

 private:
  VertexId id_ = 0;
  std::vector<EdgeId> connected_edge_ids_;
};

struct Edge {
 public:
  Edge(EdgeId id, VertexId first_vertex_id, VertexId second_vertex_id)
      : id_(id),
        first_vertex_id_(first_vertex_id),
        second_vertex_id_(second_vertex_id) {}
  EdgeId get_id() const;
  VertexId get_first_vertex_id() const;
  VertexId get_second_vertex_id() const;
  std::string to_string() const;

 private:
  EdgeId id_ = 0;
  VertexId first_vertex_id_ = 0;
  VertexId second_vertex_id_ = 0;
};

class Graph {
 public:
  void add_vertex();
  void add_edge(VertexId first_vertex_id, VertexId second_vertex_id);
  std::string to_string() const;

 private:
  std::vector<Vertex> vertices_;
  std::vector<Edge> edges_;

  VertexId get_new_vertex_id();
  EdgeId get_new_edge_id();

  Vertex& get_vertex(const VertexId& id);

  VertexId vertex_id_counter_ = 0;
  EdgeId edge_id_counter_ = 0;
};

VertexId Vertex::get_id() const {
  return id_;
}
EdgeId Edge::get_id() const {
  return id_;
}

VertexId Edge::get_first_vertex_id() const {
  return first_vertex_id_;
}

VertexId Edge::get_second_vertex_id() const {
  return second_vertex_id_;
}

void Graph::add_vertex() {
  vertices_.emplace_back(get_new_vertex_id());
}

VertexId Graph::get_new_vertex_id() {
  return vertex_id_counter_++;
}
EdgeId Graph::get_new_edge_id() {
  return edge_id_counter_++;
}

Vertex& Graph::get_vertex(const VertexId& id) {
  for (auto& vertex : vertices_) {
    if (vertex.get_id() == id) {
      return vertex;
    }
  }
  throw std::runtime_error("Vertex not found!\n");
}

const std::vector<EdgeId>& Vertex::get_connected_edge_ids() const {
  return connected_edge_ids_;
}

void Vertex::add_connected_edge_id(EdgeId id) {
  connected_edge_ids_.push_back(id);
}

void Graph::add_edge(VertexId first_vertex_id, VertexId second_vertex_id) {
  const auto& new_edge =
      edges_.emplace_back(get_new_edge_id(), first_vertex_id, second_vertex_id);
  auto& first_vertex = get_vertex(first_vertex_id);
  first_vertex.add_connected_edge_id(new_edge.get_id());
  auto& second_vertex = get_vertex(second_vertex_id);
  second_vertex.add_connected_edge_id(new_edge.get_id());
}

Graph generate_graph() {
  Graph graph;

  for (int i = 0; i < VERTICES_COUNT; ++i) {
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

std::string Vertex::to_string() const {
  std::stringstream json;
  json << "{\n  \"id\": " << get_id() << ",\n  \"edge_ids\": [";
  const auto& edge_ids = get_connected_edge_ids();
  for (const auto edge_id : edge_ids) {
    json << edge_id;
    if (edge_id != edge_ids.back()) {
      json << ", ";
    }
  }

  json << "]\n}";

  return json.str();
}

std::string Edge::to_string() const {
  std::stringstream json;
  json << "{\n  \"id\": " << get_id() << ",\n  \"vertex_ids\": ["
       << get_first_vertex_id() << ", " << get_second_vertex_id() << "]\n}";
  return json.str();
}

std::string Graph::to_string() const {
  std::stringstream json;

  json << "{\n\"vertices\": [\n";

  for (const auto& vertex : vertices_) {
    json << vertex.to_string();
    if (vertex.get_id() != vertices_.back().get_id()) {
      json << ",\n";
    }
  }

  json << "\n],\n\"edges\": [\n";

  for (const auto& edge : edges_) {
    json << edge.to_string();
    if (edge.get_id() != edges_.back().get_id()) {
      json << ",\n";
    }
  }

  json << "\n]\n}\n";

  return json.str();
}

void json_to_file(const std::string& str) {
  std::ofstream json;
  json.open("graph.json");
  if (!json.is_open()) {
    throw std::runtime_error("Can't open graph.json file");
  }
  json << str;
  json.close();
}

int main() {
  const Graph graph = generate_graph();
  const std::string json_string = graph.to_string();
  json_to_file(json_string);

  return 0;
}
