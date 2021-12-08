#include <cassert>
#include "graph.hpp"
#include <random>

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
  return vertices_at_depth_.at(depth);
}

std::set<VertexId> Graph::get_vertices_at_depth(int depth) const {
  const auto updated_depths =
      GraphTraverser::dynamic_bfs(*this, updated_depth_);
  std::set<VertexId> ans;
  for (const auto& [vertex_id, vertex_depth] : updated_depths) {
    if (vertex_depth == depth) {
      ans.insert(vertex_id);
    }
  }
  return ans;
}

void Graph::update_vertices_depth() {
  if (!is_depth_dirty_) {
    return;
  }
  const auto& depths = GraphTraverser::dynamic_bfs(*this, updated_depth_);
  update_vertices_at_depth_map(depths);
  is_depth_dirty_ = false;
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

int Graph::get_new_depth(const VertexId& vertex1_id,
                         const VertexId& vertex2_id) const {
  const auto vertex1_depth = get_vertex(vertex1_id).depth;
  const auto vertex2_depth = get_vertex(vertex2_id).depth;
  if (vertex1_depth == INIT_DEPTH && vertex2_depth == INIT_DEPTH) {
    return INIT_DEPTH;
  }
  if (vertex1_depth == INIT_DEPTH) {
    return vertex2_depth + 1;
  } else if (vertex2_depth == INIT_DEPTH) {
    return vertex1_depth + 1;
  } else {
    return std::min(vertex1_depth, vertex2_depth) + 1;
  }
}

bool Graph::new_edge_color_is_correct(const VertexId& vertex1_id,
                                      const VertexId& vertex2_id,
                                      const EdgeColor& color) {
  const auto vertices_at_depth_buffer = vertices_at_depth_;
  update_vertices_depth();
  bool is_correct;
  switch (color) {
    case EdgeColor::Gray:
      is_correct = vertices_.at(vertex1_id).connected_edges().empty() ||
                   vertices_.at(vertex2_id).connected_edges().empty();
      break;
    case EdgeColor::Green:
      is_correct = vertex1_id == vertex2_id;
      break;
    case EdgeColor::Blue:
      is_correct = get_vertex(vertex1_id).depth == get_vertex(vertex2_id).depth;
      break;
    case EdgeColor::Yellow:
      is_correct = (std::abs(get_vertex(vertex1_id).depth -
                             get_vertex(vertex2_id).depth) == 1) &&
                   !is_connected(vertex1_id, vertex2_id);
      break;
    case EdgeColor::Red:
      is_correct = std::abs(get_vertex(vertex1_id).depth -
                            get_vertex(vertex2_id).depth) == 2;
      break;
  }
  vertices_at_depth_ = vertices_at_depth_buffer;
  return is_correct;
}

void Graph::update_vertices_at_depth_map(
    const std::map<VertexId, int>& depths) {
  if (updated_depth_ == INIT_DEPTH) {
    vertices_at_depth_.clear();
  } else {
    vertices_at_depth_.erase(vertices_at_depth_.find(updated_depth_),
                             vertices_at_depth_.end());
  }
  for (const auto& [vertex_id, depth] : depths) {
    get_vertex(vertex_id).depth = depth;
    vertices_at_depth_[depth].insert(vertex_id);
  }
}

VertexId get_random_vertex_id(const std::set<VertexId>& vertex_id_set) {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> dist(0, (int)vertex_id_set.size() - 1);
  auto vertex_id_set_it = vertex_id_set.begin();
  std::advance(vertex_id_set_it, dist(gen));
  return *vertex_id_set_it;
}
