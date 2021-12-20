#include "graph.hpp"

VertexId Graph::get_last_added_vertex_id() const {
  return vertices_.back().get_id();
}

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

void GraphGenerator::generate_gray_edges(Graph& graph) const {
  std::random_device rd;
  std::mt19937 gen(rd());
  for (int i = 0; i < params_.depth(); ++i) {
    std::vector<Vertex> new_vertices;
    for (const auto& vertex : graph.get_vertices()) {
      std::bernoulli_distribution d(1.0 - (double)i / (double)params_.depth());
      if (vertex.get_level() == i && d(gen)) {
        new_vertices.push_back(vertex);
      }
    }
    for (int i = 0; i < new_vertices.size(); ++i) {
      for (int j = 0; j < params_.new_vertices_count(); ++j) {
        graph.add_vertex(new_vertices[i].get_level() + 1);
        graph.add_edge(new_vertices[i].get_id(),
                       graph.get_last_added_vertex_id(), Edge::Color::Gray);
      }
    }
  }
}

void GraphGenerator::generate_green_edges(Graph& graph) const {
  std::random_device rd;
  std::mt19937 gen(rd());
  for (const auto& vertex : graph.get_vertices()) {
    std::bernoulli_distribution d(0.1);
    if (d(gen)) {
      graph.add_edge(vertex.get_id(), vertex.get_id(), Edge::Color::Green);
    }
  }
}
void GraphGenerator::generate_yellow_edges(Graph& graph) const {
  std::random_device rd;
  std::mt19937 gen(rd());
  for (const auto& first_vertex : graph.get_vertices()) {
    std::bernoulli_distribution d((double)first_vertex.get_level() /
                                  ((double)params_.depth() - 1.0));
    if (first_vertex.get_level() < graph.get_depth() && d(gen)) {
      std::vector<VertexId> second_vertices_ids;
      for (const auto& second_vertex : graph.get_vertices()) {
        bool flag = true;
        for (const auto& connected_edge :
             first_vertex.get_connected_edge_ids()) {
          if (graph.get_edge(connected_edge).get_first_vertex_id() ==
                  second_vertex.get_id() ||
              graph.get_edge(connected_edge).get_second_vertex_id() ==
                  second_vertex.get_id() ||
              second_vertex.get_level() != first_vertex.get_level() + 1) {
            flag = false;
            break;
          }
        }

        if (flag) {
          second_vertices_ids.push_back(second_vertex.get_id());
        }
      }

      if (second_vertices_ids.size() > 0) {
        VertexId second_vertex_id =
            second_vertices_ids[rand() % second_vertices_ids.size()];
        graph.add_edge(first_vertex.get_id(), second_vertex_id,
                       Edge::Color::Yellow);
      }
    }
  }
}
void GraphGenerator::generate_red_edges(Graph& graph) const {
  std::random_device rd;
  std::mt19937 gen(rd());
  for (const auto& first_vertex : graph.get_vertices()) {
    std::bernoulli_distribution d(0.33);
    if (first_vertex.get_level() < graph.get_depth() - 1 && d(gen)) {
      std::vector<VertexId> second_vertices_ids;
      for (const auto& second_vertex : graph.get_vertices()) {
        if (second_vertex.get_level() == first_vertex.get_level() + 2) {
          second_vertices_ids.push_back(second_vertex.get_id());
        }
      }
      if (second_vertices_ids.size() > 0) {
        VertexId second_vertex_id =
            second_vertices_ids[rand() % second_vertices_ids.size()];
        graph.add_edge(first_vertex.get_id(), second_vertex_id,
                       Edge::Color::Red);
      }
    }
  }
}

void Graph::add_vertex(int level) {
  vertices_.emplace_back(get_new_vertex_id(), level);
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

void Graph::add_edge(VertexId first_vertex_id,
                     VertexId second_vertex_id,
                     Edge::Color color) {
  const auto& new_edge = edges_.emplace_back(get_new_edge_id(), first_vertex_id,
                                             second_vertex_id, color);
  auto& first_vertex = get_vertex(first_vertex_id);
  first_vertex.add_connected_edge_id(new_edge.get_id());
  auto& second_vertex = get_vertex(second_vertex_id);
  second_vertex.add_connected_edge_id(new_edge.get_id());
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
