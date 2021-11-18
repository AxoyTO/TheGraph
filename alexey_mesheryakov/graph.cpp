#include <assert.h>
#include <array>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

constexpr int VERTEX_COUNT = 14;
constexpr int EDGE_COUNT = 18;
constexpr int INVALID_ID = -1;

using std::pair;
using std::vector;
using EdgeId = int;
using VertexId = int;

class Graph {
 public:
  class Edge {
   public:
    Edge(const pair<VertexId, VertexId>& new_vertex_ids, const EdgeId& edge_id)
        : vertex_ids(new_vertex_ids), id(edge_id) {}

    std::string to_string() const {
      std::stringstream buffer;
      buffer << "{\"id\":" << id << ",\"vertex_ids\":[" << vertex_ids.first
             << "," << vertex_ids.second << "]}";
      return buffer.str();
    }
    const pair<VertexId, VertexId> vertex_ids = {};
    const EdgeId id = INVALID_ID;
  };
  class Vertex {
   public:
    explicit Vertex(const VertexId& new_id) : id(new_id) {}

    std::string to_string() const {
      std::stringstream buffer;
      buffer << "{\"id\":" << id << ",\"edge_ids\":[";
      for (int i = 0; i < edge_ids_.size() - 1; i++)
        buffer << edge_ids_[i] << ",";
      buffer << edge_ids_[edge_ids_.size() - 1] << "]}";
      return buffer.str();
    }

    void add_edge_id(const EdgeId& edge_id) {
      assert(!has_edge_id(edge_id) && "Edge id already exist");
      edge_ids_.push_back(edge_id);
    }

    bool has_edge_id(const EdgeId& new_edge_id) const {
      for (const auto& edge_id : edge_ids_)
        if (edge_id == new_edge_id) {
          return true;
        }
      return false;
    }
    const vector<EdgeId>& get_edge_ids() const { return edge_ids_; }
    const VertexId id = INVALID_ID;

   private:
    vector<EdgeId> edge_ids_ = {};
  };

  bool vertex_exist(const VertexId& id) const {
    for (const auto& vertex : vertices_)
      if (vertex.id == id)
        return true;
    return false;
  }

  bool edge_exist(const VertexId& first, const VertexId& second) const {
    for (const auto& edge_id_from_first_vertex :
         vertices_[first].get_edge_ids())
      for (const auto& edge_id_from_second_vertex :
           vertices_[second].get_edge_ids())
        if (edge_id_from_first_vertex == edge_id_from_second_vertex)
          return true;
    return false;
  }

  void add_vertex() { vertices_.emplace_back(get_new_vertex_id()); }

  void add_edge(const VertexId& first, const VertexId& second) {
    assert(vertex_exist(first) && "Source vertex id doesn't exist");
    assert(vertex_exist(second) && "Destination vertex id doesn't exist");
    assert(!edge_exist(first, second) && "Such edge already exist");
    const auto& new_edge = edges_.emplace_back(
        pair<VertexId, VertexId>{first, second}, get_new_edge_id());
    vertices_[first].add_edge_id(new_edge.id);
    vertices_[second].add_edge_id(new_edge.id);
  }

  std::string to_json() const {
    std::stringstream buffer;
    buffer << "{\"vertices\":[";
    for (int j = 0; j < vertices_.size(); j++) {
      const Vertex vertex = vertices_[j];
      buffer << vertex.to_string();
      if (j != vertices_.size() - 1)
        buffer << ",";
    }
    buffer << "],\"edges\":[";
    for (int j = 0; j < edges_.size(); j++) {
      const Edge edge = edges_[j];
      buffer << edge.to_string();
      if (j != edges_.size() - 1)
        buffer << ",";
    }
    buffer << "]}\n";
    return buffer.str();
  }

 private:
  vector<Edge> edges_ = {};
  vector<Vertex> vertices_ = {};
  VertexId vertex_id_counter_ = 0;
  EdgeId edge_id_counter_ = 0;

  EdgeId get_new_edge_id() { return edge_id_counter_++; }

  VertexId get_new_vertex_id() { return vertex_id_counter_++; }
};

int main() {
  Graph graph{};
  pair<VertexId, VertexId> connections[EDGE_COUNT] = {
      {0, 1},  {0, 2},  {0, 3},  {1, 4},   {1, 5},   {1, 6},
      {2, 7},  {2, 8},  {3, 9},  {4, 10},  {5, 10},  {6, 10},
      {7, 11}, {8, 11}, {9, 12}, {10, 13}, {11, 13}, {12, 13}};
  for (int i = 0; i < VERTEX_COUNT; i++) {
    graph.add_vertex();
  }
  for (const auto& connection : connections) {
    graph.add_edge(connection.first, connection.second);
  }
  std::ofstream file;
  file.open("graph.json", std::fstream::out | std::fstream::trunc);
  file << graph.to_json();
  file.close();
  return 0;
}
