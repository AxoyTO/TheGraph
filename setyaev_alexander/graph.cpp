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
  bool operator==(const Edge& right_edge) const;

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

bool Edge::operator==(const Edge& right_edge) const {
  if (get_id() == right_edge.get_id()) {
    return true;
  }
  return false;
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

std::string Graph::to_string() const {
  std::stringstream json;

  json << "{" << std::endl << "	\"vertices\":" << std::endl;

  if (vertices_.size() > 0) {
    json << " [" << std::endl;
  }

  for (const auto& vertex : vertices_) {
    json << "		{\n			\"id\": " << vertex.get_id()
         << "," << std::endl
         << "			\"edge_ids\": [";
    const auto& edge_ids = vertex.get_connected_edge_ids();
    for (const auto edge_id : edge_ids) {
      json << edge_id;
      if (edge_id != edge_ids.back()) {
        json << ", ";
      } else {
        json << "]" << std::endl;
      }
    }
    json << "		}";
    if (vertex.get_id() != vertices_.back().get_id()) {
      json << ",\n";
    } else {
      json << "\n	],\n	\"edges\": ";
    }
  }
  if (edges_.size() > 0) {
    json << "[\n";
  }
  for (const auto& edge : edges_) {
    json << "		{\n			\"id\": " << edge.get_id()
         << ",\n			\"vertex_ids\": ["
         << edge.get_first_vertex_id() << ", " << edge.get_second_vertex_id()
         << "]\n		}";
    if (!(edge == edges_.back())) {
      json << ",\n";
    } else {
      json << "\n	]\n}\n";
    }
  }

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
