#pragma once
#include <cassert>
#include <iostream>
#include <unordered_map>
#include <vector>

namespace uni_cource_cpp {
using std::cin;
using std::cout;
using std::endl;
using std::vector;

using VertexId = int;
using EdgeId = int;

enum class EdgeColor { Gray, Green, Blue, Yellow, Red };

struct Vertex {
  explicit Vertex(const VertexId& new_id) : id(new_id) {}
  const VertexId id = 0;
  int depth = 0;
  void add_edge_id(const EdgeId& id);
  const vector<EdgeId>& edge_ids() const { return edge_ids_; }
  bool has_edge_id(const EdgeId& id) const;

 private:
  vector<EdgeId> edge_ids_;
};

struct Edge {
  Edge(const EdgeId& new_id,
       const VertexId& begin_vertex,
       const VertexId& end_vertex,
       const EdgeColor color)
      : id(new_id), begin(begin_vertex), end(end_vertex), color(color) {}
  const EdgeId id = 0;
  const VertexId begin = 0;
  const VertexId end = 0;
  const EdgeColor color = EdgeColor::Gray;
};

class Graph {
 public:
  VertexId add_vertex();

  void add_edge(const VertexId& begin,
                const VertexId& end,
                const EdgeColor& color);

  int depth() const { return depth_map_.size() - 1; }

  const vector<Vertex>& vertices() const { return vertices_; }
  const vector<Edge>& edges() const { return edges_; }
  const vector<vector<VertexId>>& depth_map() const { return depth_map_; }
  bool has_vertex(const VertexId& vertex_id) const;
  bool is_connected(const VertexId& begin, const VertexId& end) const;
  Vertex& get_vertex(const VertexId& id);
  const std::vector<EdgeId>& get_colored_edges(const EdgeColor& color) const;

 private:
  void add_id_to_depth_map(const VertexId& begin, const VertexId& end);
  VertexId num_of_vrt_ = 0;
  EdgeId num_of_edg_ = 0;

  VertexId next_vertex_id() { return num_of_vrt_++; }
  EdgeId next_edge_id() { return num_of_edg_++; }

  vector<Vertex> vertices_;
  vector<Edge> edges_;
  vector<vector<VertexId>> depth_map_;
  std::unordered_map<EdgeColor, vector<EdgeId>> edges_color_map_;
};
}  // namespace uni_cource_cpp
