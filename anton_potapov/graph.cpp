#include "graph.hpp"

Graph& Graph::operator=(Graph&& other_graph) {
  other_graph.update_vertices_depth();
  vertices_ = std::move(other_graph.vertices_);
  edges_ = std::move(other_graph.edges_);
  next_vertex_id_ = std::move(other_graph.next_vertex_id_);
  next_edge_id_ = std::move(other_graph.next_edge_id_);
  vertices_at_depth_ = std::move(other_graph.vertices_at_depth_);
  return *this;
}

Graph::Graph(Graph&& other_graph) {
  other_graph.update_vertices_depth();
  vertices_ = std::move(other_graph.vertices_);
  edges_ = std::move(other_graph.edges_);
  next_vertex_id_ = std::move(other_graph.next_vertex_id_);
  next_edge_id_ = std::move(other_graph.next_edge_id_);
  vertices_at_depth_ = std::move(other_graph.vertices_at_depth_);
}

const std::set<VertexId>& Graph::get_vertices_at_depth(int depth) {
  update_vertices_depth();
  const auto& const_this = *this;
  return const_this.get_vertices_at_depth(depth);
}

const std::set<VertexId>& Graph::get_vertices_at_depth(int depth) const {
  return vertices_at_depth_.at(depth);
}

bool Graph::is_vertex_exists(const VertexId& vertex_id) const {
  return vertices_.find(vertex_id) != vertices_.end();
}

bool Graph::is_connected(const VertexId& vertex1_id,
                         const VertexId& vertex2_id) const {
  assert(is_vertex_exists(vertex1_id) && "Vertex 1 doesn't exist");
  assert(is_vertex_exists(vertex2_id) && "Vertex 2 doesn't exist");
  if (vertex1_id == vertex2_id) {
    for (const auto& vertex_edge_id :
         get_vertex(vertex1_id).connected_edges()) {
      const auto& vertex_edge = get_edge(vertex_edge_id);
      if (vertex_edge.vertex1_id == vertex_edge.vertex2_id) {
        return true;
      }
    }
    return false;
  } else {
    for (const auto& vertex1_edge_id :
         get_vertex(vertex1_id).connected_edges()) {
      if (vertices_.find(vertex2_id)->second.has_edge_id(vertex1_edge_id)) {
        return true;
      }
    }
    return false;
  }
}

VertexId Graph::add_vertex() {
  const VertexId new_vertex_id = get_next_vertex_id();
  vertices_.emplace(new_vertex_id, new_vertex_id);
  return new_vertex_id;
}

EdgeId Graph::add_edge(const VertexId& vertex1_id,
                       const VertexId& vertex2_id,
                       const EdgeColor& edge_color = EdgeColor::Gray) {
  assert(is_vertex_exists(vertex1_id) && "Vertex 1 doesn't exist");
  assert(is_vertex_exists(vertex2_id) && "Vertex 2 doesn't exist");
  assert(!is_connected(vertex1_id, vertex2_id) && "Vertices already connected");
  assert(new_edge_color_is_correct(vertex1_id, vertex2_id, edge_color) &&
         "the new edge's color is incorrect");

  const EdgeId new_edge_id = get_next_edge_id();
  edges_.emplace(new_edge_id,
                 Edge(new_edge_id, vertex1_id, vertex2_id, edge_color));

  get_vertex(vertex1_id).add_edge(new_edge_id);
  if (vertex1_id != vertex2_id) {
    get_vertex(vertex2_id).add_edge(new_edge_id);
  }

  is_depth_dirty_ = true;
  if (updated_depth_ != INIT_DEPTH) {
    updated_depth_ =
        std::min(updated_depth_, get_new_depth(vertex1_id, vertex2_id));
  }

  return new_edge_id;
}

VertexId get_random_vertex_id(const std::set<VertexId>& vertex_id_set) {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> dist(0, (int)vertex_id_set.size() - 1);
  auto vertex_id_set_it = vertex_id_set.begin();
  std::advance(vertex_id_set_it, dist(gen));
  return *vertex_id_set_it;
}
