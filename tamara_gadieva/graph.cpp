#include "graph.hpp"
#include <cassert>
#include <iostream>
#include <random>

constexpr float GREEN_EDGE_PROB = 0.1;
constexpr float RED_EDGE_PROB = 0.33;

std::string color_to_string(const Edge::Color& color) {
  switch (color) {
    case Edge::Color::Gray:
      return "gray";
    case Edge::Color::Green:
      return "green";
    case Edge::Color::Yellow:
      return "yellow";
    case Edge::Color::Red:
      return "red";
  }
}

bool Graph::has_vertex(const VertexId& id) const {
  for (const auto& vertex : vertices_)
    if (vertex.id == id)
      return true;
  return false;
}

bool Graph::have_vertices_in_depth(const int depth) const {
  return (depth < get_depth());
}

bool Vertex::has_edge_id(const EdgeId& id) const {
  for (const auto& edge_id : edge_ids_)
    if (edge_id == id)
      return true;
  return false;
}

void Vertex::add_edge_id(const EdgeId& id) {
  assert(!has_edge_id(id) && "Vertex already has this edge id");
  edge_ids_.push_back(id);
}

VertexId Graph::add_vertex(const int depth) {
  auto new_vertex_id = get_new_vertex_id();
  vertices_.emplace_back(new_vertex_id, depth);
  return new_vertex_id;
}

void Graph::add_edge(const VertexId& from_vertex_id,
                     const VertexId& to_vertex_id,
                     const Edge::Color& edge_color) {
  assert(has_vertex(from_vertex_id) && "Vertex doesn't exist");
  assert(has_vertex(to_vertex_id) && "Vertex doesn't exist");
  if (edge_color != Edge::Color::Green)
    assert(!is_connected(from_vertex_id, to_vertex_id) &&
           "Vertices are already connected");

  const auto& new_edge = edges_.emplace_back(get_new_edge_id(), from_vertex_id,
                                             to_vertex_id, edge_color);
  auto& from_vertex = get_vertex(from_vertex_id);
  auto& to_vertex = get_vertex(to_vertex_id);
  from_vertex.add_edge_id(new_edge.id);
  if (edge_color != Edge::Color::Green)
    to_vertex.add_edge_id(new_edge.id);
}

bool Graph::is_connected(const VertexId& from_vertex_id,
                         const VertexId& to_vertex_id) const {
  assert(has_vertex(from_vertex_id) && "Vertex doesn't exist");
  assert(has_vertex(to_vertex_id) && "Vertex doesn't exist");
  const auto& from_vertex = get_vertex(from_vertex_id);
  const auto& to_vertex = get_vertex(to_vertex_id);
  const auto& from_vertex_edges = from_vertex.get_edge_ids();
  const auto& to_vertex_edges = to_vertex.get_edge_ids();
  for (const auto& from_vertex_edge : from_vertex_edges)
    for (const auto& to_vertex_edge : to_vertex_edges)
      if (from_vertex_edge == to_vertex_edge)
        return true;
  return false;
}

const Vertex& Graph::get_vertex(const VertexId& id) const {
  assert(has_vertex(id) && "Vertex doesn't exists");
  for (const auto& vertex : vertices_)
    if (vertex.id == id)
      return vertex;
  throw std::runtime_error("Unreachable code");
}

Vertex& Graph::get_vertex(const VertexId& id) {
  const auto& const_self = *this;
  return const_cast<Vertex&>(const_self.get_vertex(id));
}

const std::vector<VertexId>& Graph::get_vertices_ids_in_depth(
    const int depth) const {
  assert(have_vertices_in_depth(depth) &&
         "Vertices don't exist in requested depth");
  return vertices_in_depth_[depth];
}

void Graph::fill_vertices_in_depth(const int depth, const VertexId id) {
  if (depth >= vertices_in_depth_.size())
    vertices_in_depth_.emplace_back();
  vertices_in_depth_[depth].emplace_back(id);
}

std::string Graph::json_string() const {
  std::string result_string;

  result_string = "{\n\t\"depth\": " + std::to_string(get_depth()) +
                  ",\n\t\"vertices\": [\n";

  for (int i = 0; i < vertices_.size(); i++) {
    result_string += vertices_[i].json_string();
    if (i != vertices_.size() - 1)
      result_string += ",\n";
    else
      result_string += "\n";
  }
  result_string += "\t],\n";

  result_string += "\t\"edges\": [\n";
  for (int i = 0; i < edges_.size(); i++) {
    result_string += edges_[i].json_string();
    if (i != edges_.size() - 1)
      result_string += ",\n";
    else
      result_string += "\n";
  }
  result_string += "\t]\n}\n";

  return result_string;
}

std::string Edge::json_string() const {
  return "\t\t{ \"id\": " + std::to_string(id) + ", \"vertex_ids\": [" +
         std::to_string(vertex_id1) + ", " + std::to_string(vertex_id2) +
         "], \"color\": \"" + color_to_string(color) + "\" }";
}

std::string Vertex::json_string() const {
  std::string result_string =
      "\t\t{ \"id\": " + std::to_string(id) + ", \"edge_ids\": [";

  for (int i = 0; i < edge_ids_.size(); i++) {
    result_string += std::to_string(edge_ids_[i]);
    if (i != edge_ids_.size() - 1)
      result_string += ", ";
    else
      result_string += "], \"depth\": ";
  }
  result_string += std::to_string(depth);
  result_string += " }";
  return result_string;
}

bool do_create_new_element(const float probability) {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::bernoulli_distribution d(probability);
  return d(gen);
}

VertexId get_random_vertex_id(const std::vector<VertexId>& vertices_ids) {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<int> d(0, vertices_ids.size() - 1);
  return vertices_ids[d(gen)];
}

const std::vector<VertexId> exclude_connected_vertices(
    const VertexId& id,
    const std::vector<VertexId>& vertices_ids,
    const Graph& graph) {
  std::vector<VertexId> unconnected_vertices;
  for (const auto& vertex_id : vertices_ids)
    if (!graph.is_connected(id, vertex_id))
      unconnected_vertices.emplace_back(vertex_id);
  return unconnected_vertices;
}

void generate_gray_edges(Graph& graph,
                         const int max_depth,
                         const int new_vertices_num) {
  graph.fill_vertices_in_depth(0, graph.add_vertex(0));
  for (int depth = 0; depth < max_depth - 1; depth++) {
    bool was_new_vertex_created = false;
    const float prob_of_creating_new_vertex =
        (float)(max_depth - depth) / (float)max_depth;
    for (const auto& vertex_id_in_current_depth :
         graph.get_vertices_ids_in_depth(depth))
      for (int i = 0; i < new_vertices_num; i++)
        if (do_create_new_element(prob_of_creating_new_vertex)) {
          was_new_vertex_created = true;
          const auto& vertex_id_in_new_depth = graph.add_vertex(depth + 1);
          graph.fill_vertices_in_depth(depth + 1, vertex_id_in_new_depth);
          graph.add_edge(vertex_id_in_current_depth, vertex_id_in_new_depth,
                         Edge::Color::Gray);
        }
    if (!was_new_vertex_created) {
      break;
    }
  }
}

void generate_green_edges(Graph& graph) {
  for (int depth = 0; depth < graph.get_depth(); depth++)
    for (const auto& vertex_id_in_current_depth :
         graph.get_vertices_ids_in_depth(depth))
      if (do_create_new_element(GREEN_EDGE_PROB))
        graph.add_edge(vertex_id_in_current_depth, vertex_id_in_current_depth,
                       Edge::Color::Green);
}

void generate_yellow_edges(Graph& graph) {
  for (int depth = graph.get_depth() - 1; depth >= 0; depth--) {
    const float prob_of_creating_new_edge =
        (float)depth / (float)(graph.get_depth() - 1);
    const auto& vertices_ids_in_current_depth =
        graph.get_vertices_ids_in_depth(depth);
    const auto& vertices_ids_in_prev_depth =
        graph.get_vertices_ids_in_depth(depth - 1);
    for (const auto& vertex_id_in_current_depth :
         vertices_ids_in_current_depth) {
      if (do_create_new_element(prob_of_creating_new_edge)) {
        auto unconnected_vertices = exclude_connected_vertices(
            vertex_id_in_current_depth, vertices_ids_in_prev_depth, graph);
        if (unconnected_vertices.size() == 0)
          continue;
        auto random_vertex_id_in_prev_depth =
            get_random_vertex_id(unconnected_vertices);
        graph.add_edge(vertex_id_in_current_depth,
                       random_vertex_id_in_prev_depth, Edge::Color::Yellow);
      }
    }
  }
}

void generate_red_edges(Graph& graph) {
  for (int depth = 0; depth < graph.get_depth() - 2; depth++) {
    const auto& vertices_ids_in_current_depth =
        graph.get_vertices_ids_in_depth(depth);
    const auto& vertices_ids_in_next_depth =
        graph.get_vertices_ids_in_depth(depth + 2);
    for (const auto& vertex_id_in_current_depth : vertices_ids_in_current_depth)
      if (do_create_new_element(RED_EDGE_PROB))
        graph.add_edge(vertex_id_in_current_depth,
                       get_random_vertex_id(vertices_ids_in_next_depth),
                       Edge::Color::Red);
  }
}

Graph GraphGenerator::generate() const {
  Graph graph;
  if (params_.depth == 0 || params_.new_vertices_num == 0)
    return graph;
  generate_gray_edges(graph, params_.depth, params_.new_vertices_num);
  generate_green_edges(graph);
  generate_yellow_edges(graph);
  generate_red_edges(graph);
  return graph;
}