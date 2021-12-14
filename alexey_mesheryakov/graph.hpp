#pragma once
#include <assert.h>
#include <stdlib.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
constexpr int INVALID_ID = -1;

using std::pair;
using std::vector;
using EdgeId = int;
using VertexId = int;
using Depth = int;
using std::string;

class Graph {
 public:
  class Edge {
   public:
    enum class Color { Gray, Green, Red, Blue, Yellow };
    Edge(const pair<VertexId, VertexId>& new_vertex_ids,
         const EdgeId& edge_id,
         const Color& new_color = Color::Gray)
        : vertex_ids(new_vertex_ids), id(edge_id), color(new_color) {}

    std::string to_string() const;
    const EdgeId id = 0;
    const pair<VertexId, VertexId> vertex_ids = {-1, -1};
    const Color color = Color::Gray;
  };

  class Vertex {
   public:
    explicit Vertex(const VertexId& new_id) : id(new_id) {}

    std::string to_string() const;

    void add_edge_id(const EdgeId& edge_id);

    bool has_edge_id(const EdgeId& new_edge_id) const;
    const vector<EdgeId>& get_edge_ids() const { return edge_ids_; }
    const VertexId id = INVALID_ID;
    const Depth& get_depth() const { return vertex_depth_; }
    void set_depth(const Depth& depth) { vertex_depth_ = depth; }

   private:
    vector<EdgeId> edge_ids_ = {};
    Depth vertex_depth_ = 0;
  };

  bool vertex_exist(const VertexId& id) const;

  bool vertex_has_depth(const VertexId& id) const;

  bool edge_exist(const VertexId& first, const VertexId& second) const;

  VertexId add_vertex();

  void add_edge(const VertexId& first,
                const VertexId& second,
                const Edge::Color& color = Edge::Color::Gray);

  const vector<Edge>& get_edges() const { return edges_; }
  const vector<Vertex>& get_vertices() const { return vertices_; }
  const int get_depth() const { return depth_ + 1; }
  void set_vertex_depth(const VertexId& from_vertex_id,
                        const VertexId& to_vertex_id);
  const vector<VertexId>& get_vertices_at_depth(const Depth& depth) const;

 private:
  vector<Edge> edges_ = {};
  vector<Vertex> vertices_ = {};
  VertexId vertex_id_counter_ = 0;
  EdgeId edge_id_counter_ = 0;
  EdgeId get_new_edge_id() { return edge_id_counter_++; }
  vector<vector<VertexId> > levels_ = {{}};
  VertexId get_new_vertex_id() { return vertex_id_counter_++; }
  const Vertex& get_vertex(const VertexId& vertex_id) const {
    return vertices_[vertex_id];
  }
  void set_vertex_depth_by_id(const VertexId& vertex_id, const Depth& depth) {
    vertices_[vertex_id].set_depth(depth);
  }
  Depth depth_ = 0;
};
std::string color_to_string(const Graph::Edge::Color& color);