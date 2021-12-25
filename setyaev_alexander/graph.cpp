#include "graph.hpp"

const Edge& Graph::get_edge(EdgeId id) const {
  for (const auto& edge : edges_) {
    if (edge.get_id() == id) {
      return edge;
    }
  }
  throw std::runtime_error("Edge not found!\n");
}

const std::vector<Vertex>& Graph::get_vertices() const {
  return vertices_;
}

bool check_probability(double param) {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::bernoulli_distribution d(param);
  return d(gen);
}

void GraphGenerator::generate_gray_edges(Graph& graph) const {
  for (int depth = 0; depth < params_.depth(); ++depth) {
    const std::vector<VertexId> vertices = graph.get_vertex_ids_at_depth(depth);
    for (const auto& vertex : vertices) {
      if (check_probability(1.0 - (double)depth / (double)params_.depth())) {
        for (int i = 0; i < params_.new_vertices_count(); ++i) {
          auto new_vertex = graph.add_vertex();
          graph.add_edge(vertex, new_vertex.get_id());
        }
      }
    }
  }
}

void GraphGenerator::generate_green_edges(Graph& graph) const {
  for (const auto& vertex : graph.get_vertices()) {
    if (check_probability(0.1)) {
      graph.add_edge(vertex.get_id(), vertex.get_id());
    }
  }
}

const std::vector<VertexId> Graph::get_unconected_vertex_ids(
    const Vertex& vertex,
    const std::vector<VertexId>& vertices_at_depth) {
  std::vector<VertexId> vertices_ids;
  for (const auto& another_vertex : vertices_at_depth) {
    bool flag = true;
    for (const auto& connected_edge : vertex.get_connected_edge_ids()) {
      if (get_edge(connected_edge).get_first_vertex_id() == another_vertex ||
          get_edge(connected_edge).get_second_vertex_id() == another_vertex) {
        flag = false;
      }
    }

    if (flag) {
      vertices_ids.push_back(another_vertex);
    }
  }
  return vertices_ids;
}

void GraphGenerator::generate_yellow_edges(Graph& graph) const {
  for (const auto& first_vertex : graph.get_vertices()) {
    if (first_vertex.get_depth() == graph.get_depth()) {
      continue;
    }

    if (check_probability((double)first_vertex.get_depth() /
                          ((double)params_.depth() - 1.0))) {
      std::vector<VertexId> second_vertices_ids = graph.get_vertex_ids_at_depth(
          graph.get_vertex_depth(first_vertex.get_id()) + 1);
      const std::vector<VertexId> unconnected_vertex_ids =
          graph.get_unconected_vertex_ids(first_vertex, second_vertices_ids);

      if (unconnected_vertex_ids.size() > 0) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> distrib(
            0, unconnected_vertex_ids.size() - 1);
        VertexId second_vertex_id = unconnected_vertex_ids[distrib(gen)];

        graph.add_edge(first_vertex.get_id(), second_vertex_id);
      }
    }
  }
}
void GraphGenerator::generate_red_edges(Graph& graph) const {
  for (const auto& first_vertex : graph.get_vertices()) {
    if (first_vertex.get_depth() == graph.get_depth() - 1) {
      continue;
    }

    if (check_probability(0.33)) {
      std::vector<VertexId> second_vertices_ids;
      for (const auto& second_vertex : graph.get_vertices()) {
        if (second_vertex.get_depth() == first_vertex.get_depth() + 2) {
          second_vertices_ids.push_back(second_vertex.get_id());
        }
      }
      if (second_vertices_ids.size() > 0) {
        VertexId second_vertex_id =
            second_vertices_ids[rand() % second_vertices_ids.size()];
        graph.add_edge(first_vertex.get_id(), second_vertex_id);
      }
    }
  }
}

Vertex Graph::add_vertex() {
  Vertex vertex = vertices_.emplace_back(get_new_vertex_id());
  if (vertex.get_id() == 0) {
    depth_map_.push_back({0});
  }
  return vertex;
}

VertexId Graph::get_new_vertex_id() {
  return vertex_id_counter_++;
}
EdgeId Graph::get_new_edge_id() {
  return edge_id_counter_++;
}

Vertex& Graph::get_vertex(const VertexId& id) {
  for (auto& vertex : vertices_) {
    if (vertex.get_id() == id) {
      return vertex;
    }
  }
  throw std::runtime_error("Vertex not found!\n");
}

Graph::Depth Graph::get_vertex_depth(VertexId vertex_id) {
  for (int i = 0; i < depth_map_.size(); ++i) {
    for (const auto& vertex_at_depth_id : depth_map_[i]) {
      if (vertex_at_depth_id == vertex_id) {
        return i;
      }
    }
  }
  return 0;
}

void Graph::add_edge(VertexId first_vertex_id, VertexId second_vertex_id) {
  Edge::Color color = get_edge_color(first_vertex_id, second_vertex_id);
  const auto& new_edge = edges_.emplace_back(get_new_edge_id(), first_vertex_id,
                                             second_vertex_id, color);

  auto& first_vertex = get_vertex(first_vertex_id);
  first_vertex.add_connected_edge_id(new_edge.get_id());
  auto& second_vertex = get_vertex(second_vertex_id);
  if (color == Edge::Color::Grey) {
    second_vertex.change_depth(first_vertex.get_depth() + 1);
    if (depth_map_.size() < first_vertex.get_depth() + 2) {
      depth_map_.push_back({second_vertex_id});
    } else {
      depth_map_[first_vertex.get_depth() + 1].push_back(second_vertex_id);
    }
  }
  if (second_vertex.get_id() != first_vertex.get_id()) {
    second_vertex.add_connected_edge_id(new_edge.get_id());
  }
}
Edge::Color Graph::get_edge_color(VertexId from_vertex_id,
                                  VertexId to_vertex_id) {
  const auto from_vertex_depth = get_vertex_depth(from_vertex_id);
  const auto to_vertex_depth = get_vertex_depth(to_vertex_id);
  if (from_vertex_id == to_vertex_id) {
    return Edge::Color::Green;
  }
  if (get_edges(to_vertex_id).size() == 0) {
    return Edge::Color::Grey;
  }

  if (to_vertex_depth - from_vertex_depth == 1 &&
      !(is_connected(from_vertex_id, to_vertex_id))) {
    return Edge::Color::Yellow;
  }
  if (to_vertex_depth - from_vertex_depth == 2) {
    return Edge::Color::Red;
  }
  throw std::runtime_error("Failed to determine color");
}

bool Graph::is_connected(VertexId from_vertex_id, VertexId to_vertex_id) {
  for (const auto& connected_edge :
       get_vertex(from_vertex_id).get_connected_edge_ids()) {
    if (get_edge(connected_edge).get_first_vertex_id() == to_vertex_id ||
        get_edge(connected_edge).get_second_vertex_id() == to_vertex_id) {
      return true;
    }
  }
  return false;
}

std::string Graph::to_string() const {
  std::stringstream json;

  json << "{\n\"vertices\": [\n";

  for (const auto& vertex : vertices_) {
    json << vertex.to_string();
    if (vertex.get_id() != vertices_.back().get_id()) {
      json << ",\n";
    }
  }

  json << "\n],\n\"edges\": [\n";

  for (const auto& edge : edges_) {
    json << edge.to_string();
    if (edge.get_id() != edges_.back().get_id()) {
      json << ",\n";
    }
  }

  json << "\n]\n}\n";

  return json.str();
}
