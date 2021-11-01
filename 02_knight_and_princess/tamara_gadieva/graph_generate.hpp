#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using VertexId = int;
using EdgeId = int;

class Vertex {
 public:
  const VertexId id;

  Vertex(const VertexId& _id) : id(_id) {}

  void add_edge_id(const EdgeId& id);

  std::string json_string() const;

 private:
  std::vector<EdgeId> edge_ids;
};

class Edge {
 public:
  const EdgeId id;
  const VertexId vertex_id1;
  const VertexId vertex_id2;

  Edge(const EdgeId& _id,
       const VertexId& _vertex_id1,
       const VertexId& _vertex_id2)
      : id(_id), vertex_id1(_vertex_id1), vertex_id2(_vertex_id2) {}

  std::string json_string() const;
};

class Graph {
 public:
  void add_vertex();

  void add_vertex_inf(const VertexId& cur_vertex_id);

  void add_edge(const VertexId& from_vertex_id, const VertexId to_vertex_id);

  std::string json_string() const;

 private:
  std::vector<Vertex> vertices_;
  std::vector<Edge> edges_;

  VertexId vertex_id_counter = 0;
  EdgeId edge_id_counter = 0;

  VertexId get_new_vertex_id() { return vertex_id_counter++; }

  EdgeId get_cur_edge_id() { return edge_id_counter; }

  void increase_edge_id() { edge_id_counter++; }
};