#pragma once

#include "edge.hpp"
#include "vertex.hpp"

#include <random>

class Graph {
 public:
  using Depth = int;

  VertexId get_new_vertex_id();
  EdgeId get_new_edge_id();

  Vertex add_vertex(int vertex_depth);
  void add_edge(VertexId first_vertex_id, VertexId second_vertex_id);
  std::string to_string() const;
  const std::vector<Vertex>& get_vertices() const;
  const Edge& get_edge(EdgeId id) const;
  const std::vector<VertexId>& get_vertex_ids_at_depth(Depth depth) {
    return depth_map_[depth];
  }
  Depth get_depth() { return depth_map_.size(); }
  Depth get_vertex_depth(VertexId vertex_id);
  const std::vector<VertexId> get_unconected_vertex_ids(
      const Vertex& vertex,
      const std::vector<VertexId>& vertices_at_depth);
  std::vector<EdgeId> get_edges(VertexId vertex_id) {
    return get_vertex(vertex_id).get_connected_edge_ids();
  }
  bool is_connected(VertexId from_vertex_id, VertexId to_vertex_id);

 private:
  std::vector<Vertex> vertices_;
  std::vector<Edge> edges_;

  Edge::Color get_edge_color(VertexId from_vertex_id, VertexId to_vertex_id);

  Vertex& get_vertex(const VertexId& id);

  VertexId vertex_id_counter_ = 0;
  EdgeId edge_id_counter_ = 0;
  std::vector<std::vector<VertexId>> depth_map_;
};

class GraphGenerator {
 public:
  struct Params {
   public:
    explicit Params(Graph::Depth depth = 0, int new_vertices_count = 0)
        : depth_(depth), new_vertices_count_(new_vertices_count) {}

    Graph::Depth depth() const { return depth_; }
    int new_vertices_count() const { return new_vertices_count_; }

   private:
    Graph::Depth depth_ = 0;
    int new_vertices_count_ = 0;
  };

  explicit GraphGenerator(const Params& params = Params()) : params_(params) {}

  Graph generate() const {
    auto graph = Graph();
    graph.add_vertex(0);
    generate_gray_edges(graph);
    generate_green_edges(graph);
    generate_yellow_edges(graph);
    generate_red_edges(graph);
    return graph;
  }

 private:
  void generate_gray_edges(Graph& graph) const;
  void generate_green_edges(Graph& graph) const;
  void generate_yellow_edges(Graph& graph) const;
  void generate_red_edges(Graph& graph) const;
  const Params params_ = Params();
};

bool check_probability(double param);
