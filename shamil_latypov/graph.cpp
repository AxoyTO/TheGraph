#include "graph.hpp"
#include <cassert>
#include <stdexcept>
#include <unordered_map>
#include <vector>

namespace uni_cource_cpp {

VertexId Vertex::get_id() const {
  return id_;
}
const std::vector<EdgeId>& Vertex::get_edge_ids() const {
  return edge_ids_;
}

void Vertex::add_edge_id(const EdgeId& id) {
  assert(!has_edge_id(id) && "This edge_id has already been added\n");
  edge_ids_.push_back(id);
}

bool Vertex::has_edge_id(const EdgeId& id) const {
  for (const auto& edge_id : edge_ids_) {
    if (edge_id == id) {
      return true;
    }
  }
  return false;
}

Edge::Color Edge::get_color() const {
  return color_;
}
EdgeId Edge::get_id() const {
  return id_;
}
VertexId Edge::get_vertex1_id() const {
  return vertex1_id_;
}
VertexId Edge::get_vertex2_id() const {
  return vertex2_id_;
}

int Graph::get_depth() const {
  return depth_map_.size() - 1;
}
const std::vector<VertexId>& Graph::get_vertices_in_depth(int depth) const {
  return depth_map_[depth];
}

void Graph::add_edge(const VertexId& vertex1_id, const VertexId& vertex2_id) {
  assert(has_vertex_id(vertex1_id) && "Vertex 1 doesnt exist");
  assert(has_vertex_id(vertex2_id) && "Vertex 2 doesnt exist");
  assert(!vertices_connected(vertex1_id, vertex2_id) &&
         "Vertices are connected");

  auto& vertex1 = get_vertex(vertex1_id);
  auto& vertex2 = get_vertex(vertex2_id);
  const auto& color = calculate_edge_color(vertex1, vertex2);

  const auto& new_edge =
      edges_.emplace_back(vertex1_id, vertex2_id, get_new_edge_id(), color);
  vertex1.add_edge_id(new_edge.get_id());
  colored_edges_[color].push_back(new_edge.get_id());
  if (color != Edge::Color::Green) {
    vertex2.add_edge_id(new_edge.get_id());
  }
  if (color == Edge::Color::Gray) {
    update_vertex_depth(vertex1, vertex2);
  }
}

VertexId Graph::add_vertex() {
  if (depth_map_.size() == 0) {
    depth_map_.push_back({});
  }
  const Vertex& new_vertex = vertices_.emplace_back(get_new_vertex_id());
  depth_map_[0].push_back(new_vertex.get_id());
  return new_vertex.get_id();
}

const std::vector<Vertex>& Graph::get_vertices() const {
  return vertices_;
}
const std::vector<Edge>& Graph::get_edges() const {
  return edges_;
}
const std::vector<std::vector<VertexId>>& Graph::get_depth_map() const {
  return depth_map_;
}

const Vertex& Graph::get_vertex(const VertexId& id) const {
  assert(has_vertex_id(id) && "Vertex doesnt exist");
  for (auto& vertex : vertices_) {
    if (vertex.get_id() == id) {
      return vertex;
    }
  }
  throw std::runtime_error("Vertex doesn't exist");
}
Vertex& Graph::get_vertex(const VertexId& id) {
  const auto& const_this = *this;
  return const_cast<Vertex&>(const_this.get_vertex(id));
}

bool Graph::has_vertex_id(const VertexId& id) const {
  for (const auto& vertex : vertices_) {
    if (vertex.get_id() == id) {
      return true;
    }
  }
  return false;
}

bool Graph::vertices_connected(const VertexId& v1_id,
                               const VertexId& v2_id) const {
  assert(has_vertex_id(v1_id) && "Vertex 1 doesnt exist");
  assert(has_vertex_id(v2_id) && "Vertex 2 doesnt exist");
  // Если вершины разные, то проверяются edge_ids обеих вершин
  if (v1_id != v2_id) {
    for (const auto& edge_id1 : get_vertex(v1_id).get_edge_ids()) {
      for (const auto& edge_id2 : get_vertex(v2_id).get_edge_ids()) {
        if (edge_id1 == edge_id2) {
          return true;
        }
      }
    }
  } else {  // Иначе ищется ребро, которое ведет из вершины в саму себя
    for (const auto& edge : edges_) {
      if (edge.get_vertex1_id() == v1_id && edge.get_vertex2_id() == v2_id) {
        return true;
      }
    }
  }  // Иначе вершины не соединены
  return false;
}

int Graph::get_vertices_cnt() const {
  return vertices_.size();
}
int Graph::get_edges_cnt() const {
  return edges_.size();
}

const std::vector<EdgeId>& Graph::get_colored_edges(
    const Edge::Color& color) const {
  if (colored_edges_.find(color) == colored_edges_.end()) {
    static std::vector<EdgeId> edge_ids;
    return edge_ids;
  }

  return colored_edges_.at(color);
}

Edge::Color Graph::calculate_edge_color(const Vertex& vertex1,
                                        const Vertex& vertex2) const {
  if (vertex2.get_edge_ids().size() == 0) {
    return Edge::Color::Gray;
  } else if (vertex1.get_id() == vertex2.get_id()) {
    return Edge::Color::Green;
  } else if (vertex1.depth == vertex2.depth) {
    return Edge::Color::Blue;
  } else if (vertex1.depth == vertex2.depth - 1) {
    return Edge::Color::Yellow;
  } else if (vertex1.depth == vertex2.depth - 2) {
    return Edge::Color::Red;
  }
  throw std::runtime_error("Failed to determine edge color");
}

void Graph::update_vertex_depth(const Vertex& vertex1, Vertex& vertex2) {
  vertex2.depth = vertex1.depth + 1;

  if (depth_map_.size() <= vertex2.depth) {
    depth_map_.push_back({});
  }

  // Добавление на нужную глубину vertex2.id и удаление vertex2.id из
  // нулевой глубины
  depth_map_[vertex2.depth].push_back(vertex2.get_id());
  for (auto vertex_id_it = depth_map_[0].begin();
       vertex_id_it != depth_map_[0].end(); vertex_id_it++) {
    if (*vertex_id_it == vertex2.get_id()) {
      depth_map_[0].erase(vertex_id_it);
      break;
    }
  }
}

}  // namespace uni_cource_cpp
