#include "graph.hpp"

constexpr double GreenProbability = 0.1;
constexpr double RedProbability = 0.33;

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

bool check_probability(double probability) {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::bernoulli_distribution d(probability);
  return d(gen);
}

int get_random_index(int size) {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> distrib(0, size);
  return distrib(gen);
}

void GraphGenerator::generate_gray_edges(Graph& graph) const {
  for (int depth = 0; depth < params_.depth(); ++depth) {
    // const for not to iterate changing objects
    const std::vector<VertexId> vertices = graph.get_vertex_ids_at_depth(depth);
    for (const auto& vertex : vertices) {
      if (check_probability(1.0 - (double)depth / (double)params_.depth())) {
        for (int i = 0; i < params_.new_vertices_count(); ++i) {
          // copy for a temporary object
          const auto new_vertex = graph.add_vertex();
          graph.add_edge(vertex, new_vertex.get_id());
        }
      }
    }
  }
}

void GraphGenerator::generate_green_edges(Graph& graph) const {
  for (const auto& vertex : graph.get_vertices()) {
    if (check_probability(GreenProbability)) {
      graph.add_edge(vertex.get_id(), vertex.get_id());
    }
  }
}

std::vector<VertexId> get_unconected_vertex_ids(
    const Graph& graph,
    const Vertex& vertex,
    const std::vector<VertexId>& vertices_at_depth) {
  std::vector<VertexId> vertices_ids;
  for (const auto& another_vertex : vertices_at_depth) {
    if (!(graph.is_connected(vertex.get_id(), another_vertex))) {
      vertices_ids.push_back(another_vertex);
    }
  }
  return vertices_ids;
}

void GraphGenerator::generate_yellow_edges(Graph& graph) const {
  for (const auto& first_vertex : graph.get_vertices()) {
    if (graph.get_vertex_depth(first_vertex.get_id()) == graph.get_depth()) {
      continue;
    }

    if (check_probability(
            (double)graph.get_vertex_depth(first_vertex.get_id()) /
            ((double)params_.depth() - 1.0))) {
      const std::vector<VertexId>& second_vertices_ids =
          graph.get_vertex_ids_at_depth(
              graph.get_vertex_depth(first_vertex.get_id()) + 1);
      const std::vector<VertexId> unconnected_vertex_ids =
          get_unconected_vertex_ids(graph, first_vertex, second_vertices_ids);

      if (unconnected_vertex_ids.size() > 0) {
        const VertexId second_vertex_id =
            unconnected_vertex_ids[get_random_index(
                unconnected_vertex_ids.size() - 1)];

        graph.add_edge(first_vertex.get_id(), second_vertex_id);
      }
    }
  }
}
void GraphGenerator::generate_red_edges(Graph& graph) const {
  for (const auto& first_vertex : graph.get_vertices()) {
    if (graph.get_vertex_depth(first_vertex.get_id()) ==
        graph.get_depth() - 1) {
      continue;
    }

    if (check_probability(RedProbability)) {
      std::vector<VertexId> second_vertices_ids;
      for (const auto& second_vertex : graph.get_vertices()) {
        if (graph.get_vertex_depth(second_vertex.get_id()) ==
            graph.get_vertex_depth(first_vertex.get_id()) + 2) {
          second_vertices_ids.push_back(second_vertex.get_id());
        }
      }
      if (second_vertices_ids.size() > 0) {
        const VertexId second_vertex_id = second_vertices_ids[get_random_index(
            second_vertices_ids.size() - 1)];
        graph.add_edge(first_vertex.get_id(), second_vertex_id);
      }
    }
  }
}

Vertex Graph::add_vertex() {
  Vertex vertex = vertices_.emplace_back(get_new_vertex_id());
  if (vertex.get_id() == 0) {
    depth_map_.push_back({0});
  } else {
    depth_map_[0].push_back(vertex.get_id());
  }
  vertices_depth_[vertex.get_id()] = 0;
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

Graph::Depth Graph::get_vertex_depth(VertexId vertex_id) const {
  return vertices_depth_.at(vertex_id);
  ;
}

void Graph::add_edge(VertexId first_vertex_id, VertexId second_vertex_id) {
  Edge::Color color = get_edge_color(first_vertex_id, second_vertex_id);
  const auto& new_edge = edges_.emplace_back(get_new_edge_id(), first_vertex_id,
                                             second_vertex_id, color);

  auto& first_vertex = get_vertex(first_vertex_id);
  adjacency_list_[first_vertex_id].push_back(new_edge.get_id());
  auto& second_vertex = get_vertex(second_vertex_id);
  if (second_vertex.get_id() != first_vertex.get_id()) {
    adjacency_list_[second_vertex_id].push_back(new_edge.get_id());
  }
  if (color == Edge::Color::Grey) {
    vertices_depth_[second_vertex_id] = get_vertex_depth(first_vertex_id) + 1;
    if (depth_map_.size() < get_vertex_depth(first_vertex.get_id()) + 2) {
      depth_map_.push_back({second_vertex_id});
    } else {
      depth_map_[get_vertex_depth(first_vertex.get_id()) + 1].push_back(
          second_vertex_id);
    }
    auto depth_iterator =
        std::find(depth_map_[0].begin(), depth_map_[0].end(), second_vertex_id);
    if (depth_iterator != depth_map_[0].end()) {
      std::swap(*depth_iterator, depth_map_[0].back());

      depth_map_[0].pop_back();
    }
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

bool Graph::is_connected(VertexId from_vertex_id, VertexId to_vertex_id) const {
  for (const auto& connected_edge : adjacency_list_.at(from_vertex_id)) {
    if (get_edge(connected_edge).get_color() == Edge::Color::Green) {
      return true;
    }
    const Edge& edge = get_edge(connected_edge);
    if ((edge.get_first_vertex_id() == to_vertex_id ||
         edge.get_second_vertex_id() == to_vertex_id) &&
        to_vertex_id != from_vertex_id) {
      return true;
    }
  }
  return false;
}

Graph GraphGenerator::generate() const {
  auto graph = Graph();
  graph.add_vertex();

  generate_gray_edges(graph);

  generate_green_edges(graph);

  generate_yellow_edges(graph);

  generate_red_edges(graph);
  return graph;
}
