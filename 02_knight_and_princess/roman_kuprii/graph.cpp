#include <array>
#include <cassert>
#include <fstream>
#include <iostream>
#include <memory>
#include <random>
#include <string>
#include <vector>

#include "graph.hpp"

using std::endl;
using std::min;
using std::shared_ptr;
using std::to_string;
using std::vector;

using EdgeId = int;
using VertexId = int;

std::string color_to_string(const Color& color) {
  switch (color) {
    case Color::Gray:
      return "\"gray\" }";
    case Color::Green:
      return "\"green\" }";
    case Color::Blue:
      return "\"blue\" }";
    case Color::Yellow:
      return "\"yellow\" }";
    case Color::Red:
      return "\"red\" }";
  }
}

double get_random_number() {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_real_distribution<> dis(0, 1);
  return dis(gen);
}

std::string Edge::to_json() const {
  std::string res;
  res = "{ \"id\": ";
  res += to_string(id);
  res += ", \"vertex_ids\": [";
  res += to_string(connected_vertices[0]);
  res += ", ";
  res += to_string(connected_vertices[1]);
  res += "], \"color\": ";
  res += color_to_string(color);
  return res;
}

bool is_edge_id_included(const EdgeId& id, const vector<EdgeId>& edge_ids) {
  for (const auto& edge_id : edge_ids)
    if (id == edge_id)
      return true;
  return false;
}

std::string Vertex::to_json() const {
  std::string res;
  res = "{ \"id\": ";
  res += to_string(get_id()) + ", \"edge_ids\": [";
  for (const auto& edge_id : edges_ids_) {
    res += to_string(edge_id);
    res += ", ";
  }
  if (edges_ids_.size() > 0) {
    res.pop_back();
    res.pop_back();
  }
  res += "] }";
  return res;
}

std::string Graph::to_json() const {
  std::string res;
  res = "{ \"depth\": ";
  res += to_string(depth_);
  res += ", \"vertices\": [ ";
  for (const auto& vertex : vertices_) {
    res += vertex.to_json();
    res += ", ";
  }
  if (vertices_.size()) {
    res.pop_back();
    res.pop_back();
  }
  res += " ], \"edges\": [ ";
  for (const auto& edge : edges_) {
    res += edge.to_json();
    res += ", ";
  }
  if (edges_.size() > 0) {
    res.pop_back();
    res.pop_back();
  }
  res += " ] }\n";
  return res;
}

bool Graph::is_vertex_exist(const VertexId& vertex_id) const {
  for (const auto& vertex : vertices_) {
    if (vertex_id == vertex.get_id())
      return true;
  }
  return false;
}

bool Graph::is_connected(const VertexId& from_vertex_id,
                         const VertexId& to_vertex_id) const {
  assert(is_vertex_exist(from_vertex_id));
  assert(is_vertex_exist(to_vertex_id));

  const auto& from_vertex_edges_ids = vertices_[from_vertex_id].get_edges_ids();
  const auto& to_vertex_edges_ids = vertices_[to_vertex_id].get_edges_ids();
  for (const auto& from_vertex_edge_id : from_vertex_edges_ids)
    if (from_vertex_id == to_vertex_id) {
      const auto& connected_vertices =
          edges_[from_vertex_edge_id].connected_vertices;
      if (connected_vertices[0] == connected_vertices[1])
        return true;
    } else
      for (const auto& to_vertex_edge_id : to_vertex_edges_ids)
        if (from_vertex_edge_id == to_vertex_edge_id)
          return true;

  return false;
}

void Graph::connect_vertices(const VertexId& from_vertex_id,
                             const VertexId& to_vertex_id,
                             const bool& initialization) {
  assert(is_vertex_exist(from_vertex_id));
  assert(is_vertex_exist(to_vertex_id));
  assert(!is_connected(from_vertex_id, to_vertex_id));

  if (initialization) {
    const int minimum_depth = [&from_vertex_id, &to_vertex_id,
                               vertices = &vertices_, edges = &edges_]() {
      int min_depth = vertices->at(from_vertex_id).depth;
      for (const auto& edge_idx : vertices->at(to_vertex_id).get_edges_ids()) {
        const VertexId vert = edges->at(edge_idx).connected_vertices[0];
        min_depth = min(min_depth, vertices->at(vert).depth);
      }
      return min_depth;
    }();
    vertices_[to_vertex_id].depth = minimum_depth + 1;
    depth_ = std::max(depth_, minimum_depth + 1);
  }

  const int diff =
      vertices_[to_vertex_id].depth - vertices_[from_vertex_id].depth;

  const Color color = [&initialization, &diff, &from_vertex_id,
                       &to_vertex_id]() {
    if (initialization)
      return Color::Gray;
    else if (from_vertex_id == to_vertex_id)
      return Color::Green;
    else if (diff == 0)
      return Color::Blue;
    else if (diff == 1)
      return Color::Yellow;
    else if (diff == 2)
      return Color::Red;
    else
      return Color::Gray;
  }();

  const auto& new_edge = edges_.emplace_back(from_vertex_id, to_vertex_id,
                                             get_next_edge_id(), color);
  vertices_[from_vertex_id].add_edge_id(new_edge.id);
  if (from_vertex_id != to_vertex_id)
    vertices_[to_vertex_id].add_edge_id(new_edge.id);
}

void write_graph(const Graph& graph, const int& graph_num) {
  std::ofstream out;
  const std::string filename =
      JSON_GRAPH_FILENAME + to_string(graph_num) + ".json";
  out.open(filename, std::ofstream::out | std::ofstream::trunc);
  out << graph.to_json();
  out.close();
}

void new_vertices_generation(Graph& work_graph,
                             const int& depth,
                             const int& new_vertices_num) {
  for (int current_depth = 0; current_depth <= depth; current_depth++) {
    const double probability =
        static_cast<double>(current_depth) / static_cast<double>(depth);
    for (const auto& vertex : work_graph.get_vertices())
      if (vertex.depth == current_depth)
        for (int iter = 0; iter < new_vertices_num; iter++) {
          if (get_random_number() > probability) {
            work_graph.add_vertex();
            work_graph.connect_vertices(
                vertex.get_id(),
                work_graph.get_vertices()[work_graph.get_vertices_num() - 1]
                    .get_id(),
                true);
          }
        }
  }
}

void add_blue_edges(Graph& work_graph) {
  const int graph_depth = work_graph.get_depth();
  for (int current_depth = 1; current_depth <= graph_depth; current_depth++) {
    vector<Vertex> uni_depth_vertices;
    for (const auto& vertex : work_graph.get_vertices())
      if (vertex.depth == current_depth)
        uni_depth_vertices.emplace_back(vertex);

    std::array<VertexId, 2> adjacent_vertices = {INVALID_ID, INVALID_ID};
    for (const auto& vertex : uni_depth_vertices) {
      if (adjacent_vertices[0] == INVALID_ID) {
        adjacent_vertices[0] = vertex.get_id();
      } else if (adjacent_vertices[1] == INVALID_ID) {
        adjacent_vertices[1] = vertex.get_id();
        if (!work_graph.is_connected(adjacent_vertices[0],
                                     adjacent_vertices[1]))
          if (get_random_number() < BLUE_TRASHOULD)
            work_graph.connect_vertices(adjacent_vertices[0],
                                        adjacent_vertices[1], false);
      } else {
        adjacent_vertices[0] = adjacent_vertices[1];
        adjacent_vertices[1] = vertex.get_id();
        if (!work_graph.is_connected(adjacent_vertices[0],
                                     adjacent_vertices[1]))
          if (get_random_number() < BLUE_TRASHOULD)
            work_graph.connect_vertices(adjacent_vertices[0],
                                        adjacent_vertices[1], false);
      }
    }
  }
}

void add_green_edges(Graph& work_graph) {
  for (const auto& start_vertex : work_graph.get_vertices())
    if (!work_graph.is_connected(start_vertex.get_id(), start_vertex.get_id()))
      if (get_random_number() < GREEN_TRASHOULD)
        work_graph.connect_vertices(start_vertex.get_id(),
                                    start_vertex.get_id(), false);
}

void add_red_edges(Graph& work_graph) {
  const int graph_depth = work_graph.get_depth();
  std::random_device rd;
  std::mt19937 gen(rd());
  for (const auto& start_vertex : work_graph.get_vertices()) {
    if (get_random_number() < RED_TRASHOULD) {
      if (start_vertex.depth + 2 <= graph_depth) {
        vector<VertexId> red_vertices_ids;
        for (const auto& end_vertex : work_graph.get_vertices()) {
          if (end_vertex.depth == start_vertex.depth + 2)
            if (!work_graph.is_connected(start_vertex.get_id(),
                                         end_vertex.get_id()))
              red_vertices_ids.emplace_back(end_vertex.get_id());
        }
        if (red_vertices_ids.size() > 0) {
          std::uniform_int_distribution<> distr(0, red_vertices_ids.size() - 1);
          work_graph.connect_vertices(start_vertex.get_id(),
                                      red_vertices_ids[distr(gen)], false);
        }
      }
    }
  }
}

void add_yellow_edges(Graph& work_graph) {
  const int graph_depth = work_graph.get_depth();
  std::random_device rd;
  std::mt19937 gen(rd());
  for (const auto& start_vertex : work_graph.get_vertices()) {
    const double probability = static_cast<double>(start_vertex.depth) /
                               static_cast<double>(graph_depth);
    if (get_random_number() < probability) {
      vector<VertexId> yellow_vertices_ids;
      for (const auto& end_vertex : work_graph.get_vertices()) {
        if (end_vertex.depth == start_vertex.depth + 1) {
          if (!work_graph.is_connected(start_vertex.get_id(),
                                       end_vertex.get_id()))
            yellow_vertices_ids.push_back(end_vertex.get_id());
        }
      }
      if (yellow_vertices_ids.size() > 0) {
        std::uniform_int_distribution<> distr(0,
                                              yellow_vertices_ids.size() - 1);
        work_graph.connect_vertices(start_vertex.get_id(),
                                    yellow_vertices_ids[distr(gen)], false);
      }
    }
  }
}

void paint_edges(Graph& work_graph) {
  add_blue_edges(work_graph);
  add_green_edges(work_graph);
  add_red_edges(work_graph);
  add_yellow_edges(work_graph);
}
