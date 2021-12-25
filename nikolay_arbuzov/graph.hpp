#pragma once

#include <set>
#include <string>
#include <unordered_map>
#include <vector>

namespace uni_course_cpp {
class Graph {
 public:
  using Depth = int;
  using VertexId = int;
  using EdgeId = int;
  struct Vertex {
    explicit Vertex(const VertexId& _id) : id(_id){};
    VertexId id = 0;
  };

  struct Edge {
    enum class Color { Grey, Green, Yellow, Red };
    explicit Edge(const EdgeId& _id,
                  const Color& _color,
                  const VertexId& _from_vertex_id,
                  const VertexId& _to_vertex_id)
        : id(_id),
          color(_color),
          from_vertex_id(_from_vertex_id),
          to_vertex_id(_to_vertex_id) {}
    const Color color = Color::Grey;
    EdgeId id = 0;
    const VertexId from_vertex_id = 0;
    const VertexId to_vertex_id = 0;
  };

  Vertex& add_vertex();

  bool has_vertex(const VertexId& vertex_id) const;
  const Vertex& get_vertex(const VertexId& vertex_id) const;
  const Edge& get_edge(const EdgeId& edge_id) const;
  const Depth& get_vertex_depth(const VertexId& vertex_id) const {
    return vertices_depth_.at(vertex_id);
  }

  bool is_connected(const VertexId& from_vertex_id,
                    const VertexId& to_vertex_id) const;
  std::vector<VertexId> get_unconnected_vertex_ids(
      const VertexId& vertex_id) const;
  Edge::Color determine_color(const VertexId& from_vertex_id,
                              const VertexId& to_vertex_id) const;

  Edge& add_edge(const VertexId& from_vertex_id, const VertexId& to_vertex_id);

  bool has_edge(const EdgeId& edge_id) const;

  std::string get_edges_distribution() const;

  const std::vector<EdgeId>& get_color_edge_ids(const Edge::Color& color) const;
  const std::vector<VertexId>& get_vertex_ids_on_depth(
      const Depth& depth) const {
    return depth_map_.at(depth);
  }
  const std::vector<Vertex>& vertices() const { return vertices_; }
  const std::vector<Edge>& edges() const { return edges_; }
  Depth depth() const { return depth_map_.size() - 1; }
  const VertexId& vertices_id_counter() const { return vertex_id_counter_; }
  const EdgeId& edges_id_counter() const { return edges_id_counter_; }
  const std::set<EdgeId>& connected_edge_ids(const VertexId& id) const {
    return adjacency_list_.at(id);
  }

 private:
  std::vector<Edge> edges_;
  std::vector<Vertex> vertices_;
  std::unordered_map<VertexId, std::set<EdgeId>> adjacency_list_;
  std::unordered_map<VertexId, Depth> vertices_depth_;
  std::unordered_map<Edge::Color, std::vector<EdgeId>> color_map_;
  std::vector<std::vector<VertexId>> depth_map_;

  EdgeId edges_id_counter_ = 0;
  VertexId vertex_id_counter_ = 0;

  void set_vertex_depth(const VertexId& vertex_id, const Depth& depth);
  void update_vertex_depth(const VertexId& vertex_id, const Depth& depth);
  VertexId get_new_vertex_id() { return vertex_id_counter_++; }
  EdgeId get_new_edge_id() { return edges_id_counter_++; }
};
}  // namespace uni_course_cpp
