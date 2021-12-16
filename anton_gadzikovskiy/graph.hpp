#pragma once

#include <unordered_map>
#include <vector>

namespace uni_cource_cpp {

class Graph {
 public:
  using VertexId = int;
  using EdgeId = int;
  using Depth = int;

  struct Vertex {
   public:
    const VertexId id;
    Depth depth = 0;

    explicit Vertex(const VertexId& _id) : id(_id) {}

    void add_edge_id(const EdgeId& id) { edge_ids_.push_back(id); }

    const std::vector<EdgeId>& get_edge_ids() const { return edge_ids_; }

   private:
    std::vector<EdgeId> edge_ids_;
  };

  struct Edge {
    enum class Color { Gray, Green, Yellow, Red };

    const VertexId from_vertex_id;
    const VertexId to_vertex_id;
    const EdgeId id;
    const Color color;

    Edge(const EdgeId& _id,
         const VertexId& _from_vertex_id,
         const VertexId& _to_vertex_id,
         const Color& _color)
        : id(_id),
          from_vertex_id(_from_vertex_id),
          to_vertex_id(_to_vertex_id),
          color(_color) {}
  };

  VertexId add_vertex();

  void add_edge(const VertexId& from_vertex_id, const VertexId& to_vertex_id);

  const Edge& get_edge(const EdgeId& id) const { return edges_.at(id); }

  Vertex& get_vertex(const VertexId& id) { return vertices_.at(id); }

  Depth get_vertex_depth(const VertexId& vertex_id) const {
    return vertices_.at(vertex_id).depth;
  }

  bool is_connected(const VertexId& from_vertex_id,
                    const VertexId& to_vertex_id) const;

  Edge::Color define_edge_color(const VertexId& from_vertex_id,
                                const VertexId& to_vertex_id) const;

  const std::vector<EdgeId>& get_vertex_edge_ids(
      const VertexId& vertex_id) const {
    return vertices_.at(vertex_id).get_edge_ids();
  }

  const std::unordered_map<VertexId, Vertex>& get_vertices() const {
    return vertices_;
  }

  const std::vector<std::vector<VertexId>>& get_vertices_on_depth() const {
    return vertices_on_depth_;
  }

  const std::vector<VertexId>& get_vertex_ids_on_depth(
      const Depth& depth) const {
    return vertices_on_depth_[depth];
  }

  const std::unordered_map<EdgeId, Edge>& get_edges() const { return edges_; }

  const std::vector<EdgeId>& get_colored_edge_ids(
      const Edge::Color& color) const {
    return colored_edge_ids_.at(color);
  }

  const Depth depth() const { return vertices_on_depth_.size() - 1; }

  const int get_vertices_amount() const { return vertices_.size(); }

  const int get_edges_amount() const { return edges_.size(); }

 private:
  std::unordered_map<VertexId, Vertex> vertices_;
  std::unordered_map<EdgeId, Edge> edges_;
  std::unordered_map<Edge::Color, std::vector<EdgeId>> colored_edge_ids_;
  std::vector<std::vector<VertexId>> vertices_on_depth_;
  VertexId vertex_id_counter_ = 0;
  EdgeId edge_id_counter_ = 0;

  VertexId get_new_vertex_id() { return vertex_id_counter_++; }

  EdgeId get_new_edge_id() { return edge_id_counter_++; }
};

}  // namespace uni_cource_cpp
