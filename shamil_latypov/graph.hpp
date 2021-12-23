#pragma once

#include <unordered_map>
#include <vector>

namespace uni_cource_cpp {

using VertexId = int;
using EdgeId = int;

class Vertex {
 public:
  explicit Vertex(const VertexId& id) : id_(id) {}

  int depth;
  VertexId get_id() const;
  const std::vector<EdgeId>& get_edge_ids() const;

  void add_edge_id(const EdgeId& id);

  bool has_edge_id(const EdgeId& id) const;

 private:
  int depth_;
  VertexId id_;
  std::vector<EdgeId> edge_ids_;
};

class Edge {
 public:
  enum class Color { Gray, Green, Blue, Yellow, Red };

  Edge(const VertexId& vertex1_id,
       const VertexId& vertex2_id,
       const EdgeId& id,
       const Color& color)
      : vertex1_id_(vertex1_id),
        vertex2_id_(vertex2_id),
        id_(id),
        color_(color) {}

  Color get_color() const;
  EdgeId get_id() const;
  VertexId get_vertex1_id() const;
  VertexId get_vertex2_id() const;

 private:
  EdgeId id_;
  VertexId vertex1_id_;
  VertexId vertex2_id_;
  Color color_;
};

class Graph {
 public:
  int get_depth() const;
  const std::vector<VertexId>& get_vertices_in_depth(int depth) const;

  void add_edge(const VertexId& vertex1_id, const VertexId& vertex2_id);
  VertexId add_vertex();

  const std::vector<Vertex>& get_vertices() const;
  const std::vector<Edge>& get_edges() const;
  const std::vector<std::vector<VertexId>>& get_depth_map() const;

  const Vertex& get_vertex(const VertexId& id) const;
  Vertex& get_vertex(const VertexId& id);

  bool has_vertex_id(const VertexId& id) const;
  bool vertices_connected(const VertexId& v1_id, const VertexId& v2_id) const;

  int get_vertices_count() const;
  int get_edges_count() const;

  const std::vector<EdgeId>& get_colored_edges(const Edge::Color& color) const;

 private:
  VertexId vert_num_ = 0;
  EdgeId edge_num_ = 0;
  std::vector<Vertex> vertices_;
  std::vector<Edge> edges_;
  std::vector<std::vector<VertexId>> depth_map_;
  std::unordered_map<Edge::Color, std::vector<EdgeId>> colored_edges_;

  VertexId get_new_vertex_id() { return vert_num_++; }
  EdgeId get_new_edge_id() { return edge_num_++; }

  Edge::Color calculate_edge_color(const Vertex& vertex1,
                                   const Vertex& vertex2) const;

  void update_vertex_depth(const Vertex& vertex1, Vertex& vertex2);
};

}  // namespace uni_cource_cpp
