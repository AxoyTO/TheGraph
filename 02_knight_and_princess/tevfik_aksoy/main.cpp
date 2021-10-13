#include <cassert>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using std::to_string;
using std::vector;

using VertexId = int;
using EdgeId = int;

struct Edge;
struct Vertex;
bool vertex_exists_in_graph(const VertexId& id, const vector<Vertex>& vertices);
bool edge_exists_in_graph(const EdgeId& id, const vector<Edge>& edges);
bool edge_id_exists_in_vertex(const EdgeId& id, const vector<EdgeId>& edge_ids);
bool is_vertex_id_valid(const VertexId& id);
bool is_edge_id_valid(const EdgeId& id);

struct Vertex {
 public:
  const VertexId id;

  explicit Vertex(const VertexId& _id) : id(_id) {}

  void add_edge_id(const EdgeId& _id) {
    assert(!edge_id_exists_in_vertex(_id, edge_ids_) &&
           "Edge already exists in vertex!");
    edge_ids_.push_back(_id);
  }

  std::string to_JSON() const {
    std::string json_string;

    json_string += "\t{ \"id\": " + to_string(id) + ", \"edge_ids\": [";
    for (int i = 0; i < edge_ids_.size(); i++) {
      json_string += to_string(edge_ids_[i]);
      if (i + 1 != edge_ids_.size())
        json_string += ", ";
    }
    return json_string;
  }

  const vector<EdgeId>& get_edge_ids() const { return edge_ids_; }

 private:
  vector<EdgeId> edge_ids_;
};

struct Edge {
 public:
  const EdgeId id;
  const VertexId source;
  const VertexId destination;

  Edge(const VertexId& src_id, const VertexId& dest_id, const EdgeId& _id)
      : id(_id), source(src_id), destination(dest_id) {}

  std::string to_JSON() const {
    std::string json_string;
    json_string += "\t{ \"id\": " + to_string(id) + ", \"vertex_ids\": [" +
                   to_string(source) + ", " + to_string(destination) + "] }";
    return json_string;
  }
};

class Graph {
 public:
  void insert_node(const VertexId& vertex) {
    assert(is_vertex_id_valid(vertex) && "Vertex id is not valid!");
    assert(!vertex_exists_in_graph(vertex, vertices_) &&
           "Vertex already exists!");
    vertices_.emplace_back(vertex);
  }
  void insert_edge(const VertexId& source,
                   const VertexId& destination,
                   const EdgeId& id) {
    assert(is_edge_id_valid(id) && "Edge id is not valid!");
    assert(!edge_exists_in_graph(id, edges_) && "Edge already exists!");
    assert(!are_vertices_connected(source, destination) &&
           "Vertices are already connected!");
    assert(vertex_exists_in_graph(source, vertices_) &&
           "Source vertex id doesn't exist!");
    assert(vertex_exists_in_graph(destination, vertices_) &&
           "Destination vertex id doesn't exist!");

    edges_.emplace_back(source, destination, id);
    vertices_[source].add_edge_id(id);
    vertices_[destination].add_edge_id(id);
  }

  bool are_vertices_connected(const VertexId& source_vertex,
                              const VertexId& destination_vertex) {
    for (const auto& source : vertices_[source_vertex].get_edge_ids()) {
      for (const auto& destination :
           vertices_[destination_vertex].get_edge_ids()) {
        if (source == destination) {
          std::cerr << "Vertices: " << source_vertex << " and "
                    << destination_vertex
                    << " are already connected with edge: " << source << "\n";
          return true;
        }
      }
    }
    return false;
  }

  std::string to_JSON() const {
    std::string json_string;
    json_string += "{\n\"vertices\": [\n";
    for (int i = 0; i < vertices_.size(); i++) {
      json_string += vertices_[i].to_JSON();
      if (i + 1 == vertices_.size()) {
        json_string += "] }\n  ],\n";
      } else {
        json_string += "] },\n";
      }
    }

    json_string += "\"edges\": [\n";
    for (int i = 0; i < edges_.size(); i++) {
      json_string += edges_[i].to_JSON();
      if (i + 1 == edges_.size()) {
        json_string += "\n";
      } else {
        json_string += ",\n";
      }
    }
    json_string += "  ]\n}\n";
    return json_string;
  }

 private:
  vector<Edge> edges_;
  vector<Vertex> vertices_;
};

const Graph generateGraph() {
  Graph graph;
  for (int i = 0; i < 14; i++) {
    graph.insert_node(i);
  }
  graph.insert_edge(0, 1, 0);
  graph.insert_edge(0, 2, 1);
  graph.insert_edge(0, 3, 2);
  graph.insert_edge(1, 4, 3);
  graph.insert_edge(1, 5, 4);
  graph.insert_edge(1, 6, 5);
  graph.insert_edge(2, 7, 6);
  graph.insert_edge(2, 8, 7);
  graph.insert_edge(3, 9, 8);
  graph.insert_edge(4, 10, 9);
  graph.insert_edge(5, 10, 10);
  graph.insert_edge(6, 10, 11);
  graph.insert_edge(7, 11, 12);
  graph.insert_edge(8, 11, 13);
  graph.insert_edge(9, 12, 14);
  graph.insert_edge(10, 13, 15);
  graph.insert_edge(11, 13, 16);
  graph.insert_edge(12, 13, 17);
  return graph;
}

bool edge_exists_in_graph(const EdgeId& id, const vector<Edge>& edges) {
  for (const auto& edge : edges)
    if (edge.id == id) {
      std::cerr << "There already exists edge: " << id << "\n";
      return true;
    }
  return false;
}
bool vertex_exists_in_graph(const VertexId& id,
                            const vector<Vertex>& vertices) {
  for (const auto& vertex : vertices)
    if (vertex.id == id) {
      return true;
    }
  return false;
}

bool edge_id_exists_in_vertex(const EdgeId& edge_id,
                              const vector<EdgeId>& edge_ids) {
  for (const auto& edge : edge_ids) {
    if (edge_id == edge) {
      std::cerr << "There already exists edge: " << edge_id
                << " in this Vertex!\n";
      return true;
    }
  }
  return false;
}

bool is_vertex_id_valid(const VertexId& id) {
  if (id < 0) {
    return false;
  }
  return true;
}

bool is_edge_id_valid(const EdgeId& id) {
  if (id < 0) {
    return false;
  }
  return true;
}

int main() {
  const auto graph = generateGraph();
  std::ofstream file("graph.json", std::ios::out);
  if (!file.is_open())
    std::cerr << "Error opening the file graph.json!\n";
  else {
    file << graph.to_JSON();
    file.close();
  }
  return 0;
}
