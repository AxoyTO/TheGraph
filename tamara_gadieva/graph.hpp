#pragma once
#include <array>
#include <string>
#include <vector>

using VertexId = int;
using EdgeId = int;

class Vertex {
 public:
  const VertexId id;
  int depth = 0;

  explicit Vertex(const VertexId& _id) : id(_id) {}

  void add_edge_id(const EdgeId& id);

  bool has_edge_id(const EdgeId& id) const;

  const std::vector<EdgeId>& get_edge_ids() const { return edge_ids_; }

  std::string json_string() const;

 private:
  std::vector<EdgeId> edge_ids_;
};

class Edge {
 public:
  const EdgeId id;
  const VertexId vertex_id1;
  const VertexId vertex_id2;
  enum class Color { Gray, Green, Yellow, Red };
  const Color color;

  Edge(const EdgeId& _id,
       const VertexId& _vertex_id1,
       const VertexId& _vertex_id2,
       const Color& _color)
      : id(_id),
        vertex_id1(_vertex_id1),
        vertex_id2(_vertex_id2),
        color(_color) {}

  std::string json_string() const;
};

class Graph {
 public:
  VertexId add_vertex();

  void add_edge(const VertexId& from_vertex_id, const VertexId& to_vertex_id);

  Edge::Color define_edge_color(const VertexId& from_vertex_id,
                                const VertexId& to_vertex_id);

  bool has_vertex(const VertexId& id) const;

  bool is_connected(const VertexId& from_vertex_id,
                    const VertexId& to_vertex_id) const;

  const Vertex& get_vertex(const VertexId& id) const;

  Vertex& get_vertex(const VertexId& id);

  const Edge& get_edge(const EdgeId& id) const;

  void update_vertex_depth(const VertexId& id, const int depth);

  const std::vector<VertexId>& get_vertices_ids_in_depth(int depth) const;

  const std::vector<EdgeId>& get_edge_ids(const VertexId& id) const;

  const int get_depth() const { return vertices_ids_in_depth_.size(); }

  std::string json_string() const;

 private:
  std::vector<Vertex> vertices_;
  std::vector<Edge> edges_;
  std::vector<std::vector<VertexId>> vertices_ids_in_depth_;

  VertexId vertex_id_counter_ = 0;
  EdgeId edge_id_counter_ = 0;

  VertexId get_new_vertex_id() { return vertex_id_counter_++; }

  EdgeId get_new_edge_id() { return edge_id_counter_++; }
};

class GraphGenerator {
 public:
  struct Params {
    const int depth = 0;
    const int new_vertices_num = 0;

    explicit Params(int _depth = 0, int _new_vertices_num = 0)
        : depth(_depth), new_vertices_num(_new_vertices_num) {}
  };

  explicit GraphGenerator(const Params& params = Params()) : params_(params) {}

  Graph generate() const;

 private:
  const Params params_ = Params();
};
