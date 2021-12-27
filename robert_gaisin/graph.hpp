#pragma once

#include <unordered_map>
#include <vector>

namespace uni_course_cpp {

using VertexId = int;
using EdgeId = int;

enum class EdgeColor { Gray, Green, Blue, Yellow, Red };

struct Vertex {
  explicit Vertex(const VertexId& new_id) : id(new_id) {}
  const VertexId id = 0;
  int depth = 0;
  void add_edge_id(const EdgeId& id);
  const std::vector<EdgeId>& edge_ids() const { return edge_ids_; }
  bool has_edge_id(const EdgeId& id) const;

 private:
  std::vector<EdgeId> edge_ids_;
};

struct Edge {
  using Duration = int;
  Edge(const EdgeId& new_id,
       const VertexId& begin_vertex,
       const VertexId& end_vertex,
       const EdgeColor _color);
  const EdgeId id = 0;
  const VertexId begin = 0;
  const VertexId end = 0;
  const EdgeColor color = EdgeColor::Gray;
  const Duration& duration() const { return duration_; }

 private:
  Duration duration_;
};

class Graph {
 public:
  VertexId add_vertex();

  void add_edge(const VertexId& begin,
                const VertexId& end,
                const EdgeColor& color);

  int depth() const { return depth_map_.size() - 1; }

  const std::vector<Vertex>& vertices() const { return vertices_; }
  const std::vector<Edge>& edges() const { return edges_; }
  const std::vector<std::vector<VertexId>>& depth_map() const {
    return depth_map_;
  }
  bool has_vertex(const VertexId& vertex_id) const;
  bool is_connected(const VertexId& begin, const VertexId& end) const;

  const Vertex& get_vertex(const VertexId& id) const;
  const Edge& get_edge(const EdgeId& edge_id) const;
  const std::vector<EdgeId>& get_colored_edges(const EdgeColor& color) const;
  std::vector<VertexId> get_linked_vertex_ids(const VertexId& vertex_id) const;

 private:
  void add_id_to_depth_map(const VertexId& begin, const VertexId& end);
  VertexId num_of_vrt_ = 0;
  EdgeId num_of_edg_ = 0;
  VertexId next_vertex_id() { return num_of_vrt_++; }
  EdgeId next_edge_id() { return num_of_edg_++; }
  Vertex& vertex(const VertexId& id);

  std::vector<Vertex> vertices_;
  std::vector<Edge> edges_;
  std::vector<std::vector<VertexId>> depth_map_;
  std::unordered_map<EdgeColor, std::vector<EdgeId>> edges_color_map_;
};
}  // namespace uni_course_cpp
