#pragma once

#include <array>
#include <cassert>
#include <fstream>
#include <iostream>
#include <memory>
#include <random>
#include <string>
#include <vector>

using std::endl;
using std::min;
using std::shared_ptr;
using std::to_string;
using std::vector;

using EdgeId = int;
using VertexId = int;

constexpr double GREEN_TRASHOULD = 0.1;
constexpr double BLUE_TRASHOULD = 0.25;
constexpr double RED_TRASHOULD = 0.33;
constexpr int INVALID_ID = -1;
const std::string JSON_GRAPH_FILENAME = "temp/graph_";

enum class Color { Gray, Green, Blue, Yellow, Red };

std::string color_to_string(const Color& color);
double get_random_number();

struct Edge {
  const EdgeId id = INVALID_ID;
  const std::array<VertexId, 2> connected_vertices;
  const Color color = Color::Gray;

  Edge(const VertexId& start,
       const VertexId& end,
       const EdgeId& _id,
       const Color& _color)
      : id(_id), connected_vertices({start, end}), color(_color) {}

  std::string to_json() const;
};

bool is_edge_id_included(const EdgeId& id, const vector<EdgeId>& edge_ids);

struct Vertex {
 public:
  int depth = 0;

  explicit Vertex(const VertexId& _id) : id_(_id) {}

  std::string to_json() const;

  void add_edge_id(const EdgeId& _id) {
    assert(!is_edge_id_included(_id, edges_ids_));
    edges_ids_.push_back(_id);
  }

  const vector<EdgeId>& get_edges_ids() const { return edges_ids_; }

  const VertexId& get_id() const { return id_; }

 private:
  const VertexId id_ = INVALID_ID;
  vector<EdgeId> edges_ids_;
};

class Graph {
 public:
  std::string to_json() const;

  void add_vertex() { vertices_.emplace_back(get_next_vertex_id()); }

  bool is_vertex_exist(const VertexId& vertex_id) const;

  bool is_connected(const VertexId& from_vertex_id,
                    const VertexId& to_vertex_id) const;

  void connect_vertices(const VertexId& from_vertex_id,
                        const VertexId& to_vertex_id,
                        const bool& initialization);

  const vector<Edge>& get_edges() const { return edges_; }
  const vector<Vertex>& get_vertices() const { return vertices_; }

  int get_depth() const { return depth_; }
  int get_vertices_num() const { return vertices_.size(); }
  int get_edges_num() const { return edges_.size(); }

 private:
  vector<Vertex> vertices_;
  vector<Edge> edges_;
  int depth_ = 0;
  VertexId vertex_id_counter_ = 0;
  EdgeId edge_id_counter_ = 0;

  VertexId get_next_vertex_id() { return vertex_id_counter_++; }
  VertexId get_next_edge_id() { return edge_id_counter_++; }
};

void write_graph(const Graph& graph, const int& graph_num);
void new_vertices_generation(Graph& work_graph,
                             const int& depth,
                             const int& new_vertices_num);

void add_blue_edges(Graph& work_graph);
void add_green_edges(Graph& work_graph);
void add_red_edges(Graph& work_graph);
void add_yellow_edges(Graph& work_graph);

void paint_edges(Graph& work_graph);
