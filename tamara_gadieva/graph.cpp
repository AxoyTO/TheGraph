#include "graph.hpp"
#include <cassert>
#include <stdexcept>

bool Graph::has_vertex(const VertexId& id) const {
  for (const auto& vertex : vertices_)
    if (vertex.id == id)
      return true;
  return false;
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

void Graph::add_vertex() {
  vertices_.emplace_back(get_new_vertex_id());
}

void Graph::add_edge(const VertexId& from_vertex_id,
                     const VertexId& to_vertex_id) {
  assert(has_vertex(from_vertex_id) && "Vertex doesn't exist");
  assert(has_vertex(to_vertex_id) && "Vertex doesn't exist");
  assert(!is_connected(from_vertex_id, to_vertex_id) &&
         "Vertices are already connected");

  const auto& new_edge =
      edges_.emplace_back(get_new_edge_id(), from_vertex_id, to_vertex_id);
  auto& from_vertex = get_vertex(from_vertex_id);
  auto& to_vertex = get_vertex(to_vertex_id);
  from_vertex.add_edge_id(new_edge.id);
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

std::string Graph::json_string() const {
  std::string result_string;

  result_string += "{\n";

  result_string += "\t\"vertices\": [\n";
  for (int i = 0; i < vertices_.size(); i++) {
    result_string += vertices_[i].json_string();
    if (i != vertices_.size() - 1)
      result_string += ",";
    result_string += "\n";
  }
  result_string += "\n\t],\n";

  result_string += "\t\"edges\": [\n";
  for (int i = 0; i < edges_.size(); i++) {
    result_string += edges_[i].json_string();
    if (i != edges_.size() - 1)
      result_string += ",";
    result_string += "\n";
  }
  result_string += "\n\t]\n";

  result_string += "}\n";

  return result_string;
}

std::string Edge::json_string() const {
  return "\t\t{ \"id\": " + std::to_string(id) + ", \"vertex_ids\": [" +
         std::to_string(vertex_id1) + ", " + std::to_string(vertex_id2) + "] }";
}

std::string Vertex::json_string() const {
  std::string result_string =
      "\t\t{ \"id\": " + std::to_string(id) + ", \"edge_ids\": [";

  for (int i = 0; i < edge_ids_.size(); i++) {
    result_string += std::to_string(edge_ids_[i]);
    if (i != edge_ids_.size() - 1)
      result_string += ", ";
    else
      result_string += "] }";
  }

  return result_string;
}
