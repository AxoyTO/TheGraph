#ifndef GRAPH
#define GRAPH

#include "edge.hpp"
#include "vertex.hpp"

#include <random>

class Graph {
 public:
  using Depth = int;
  explicit Graph(Depth depth) : depth_(depth) {}
  void add_vertex(int level);
  void add_edge(VertexId first_vertex_id,
                VertexId second_vertex_id,
                Edge::Color color);
  std::string to_string() const;
  const std::vector<Vertex>& get_vertices() const;
  VertexId get_last_added_vertex_id() const;
  const Edge& get_edge(EdgeId id) const;
  Depth get_depth() const { return depth_; }

 private:
  std::vector<Vertex> vertices_;
  std::vector<Edge> edges_;

  VertexId get_new_vertex_id();
  EdgeId get_new_edge_id();

  Vertex& get_vertex(const VertexId& id);

  VertexId vertex_id_counter_ = 0;
  EdgeId edge_id_counter_ = 0;
  Depth depth_ = 0;
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
    auto graph = Graph(params_.depth());
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

#endif
