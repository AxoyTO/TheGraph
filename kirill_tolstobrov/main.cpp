#include <algorithm>
#include <cassert>
#include <chrono>
#include <fstream>
#include <iostream>
#include <iterator>
#include <map>
#include <random>
#include <vector>

constexpr float GREEN_PROB = 0.1;
constexpr float BLUE_PROB = 0.25;
constexpr float RED_PROB = 0.33;

using VertexId = int;
using EdgeId = int;

class Vertex {
 public:
  explicit Vertex(const VertexId& init_id = 0) : id(init_id) {}
  const std::vector<EdgeId>& get_edge_ids() const { return edge_ids_; }
  bool check_edge_presence(const EdgeId& edge_id) const {
    for (const auto& id : edge_ids_) {
      if (edge_id == id) {
        return true;
      }
    }
    return false;
  }
  void add_edge_id(const EdgeId& edge_id) {
    assert(!check_edge_presence(edge_id) &&
           "Attemptig to add added edge to vertex: Error.");
    edge_ids_.push_back(edge_id);
  }
  operator std::string() const {
    std::string result = "";
    result += "{\n\"id\": " + std::to_string(id);
    result += ",\n\"edge_ids\": [";

    for (int i = 0; i < edge_ids_.size(); i++) {
      result += std::to_string(edge_ids_[i]);
      if (i != edge_ids_.size() - 1) {
        result += ", ";
      }
    }
    result += "],\n";

    result += "\"depth\": " + std::to_string(depth);
    result += "\n}";
    return result;
  }

  const VertexId id;
  int depth = 0;

 private:
  std::vector<EdgeId> edge_ids_;
};

class Edge {
 public:
  enum class Color { Grey, Green, Blue, Yellow, Red };

  Edge(const EdgeId& init_id,
       const VertexId& v1_init,
       const VertexId& v2_init,
       const Color& color_init)
      : id(init_id),
        vertex1_id(v1_init),
        vertex2_id(v2_init),
        color(color_init){};

  operator std::string() const;

  const EdgeId id;
  const VertexId vertex1_id;
  const VertexId vertex2_id;
  const Color color;
};

std::string color_to_string(Edge::Color color) {
  switch (color) {
    case Edge::Color::Grey:
      return "grey";
    case Edge::Color::Green:
      return "green";
    case Edge::Color::Blue:
      return "blue";
    case Edge::Color::Yellow:
      return "yellow";
    case Edge::Color::Red:
      return "red";
  }
  throw std::runtime_error("Nonexistent color");
}

Edge::operator std::string() const {
  std::string result = "";
  result += "{\n\"id\": " + std::to_string(id);
  result += ",\n\"vertex_ids\": ";

  result += "[" + std::to_string(vertex1_id);
  result += ", " + std::to_string(vertex2_id);
  result += "],\n\"color\": \"";
  result += color_to_string(color);
  result += "\"\n}";

  return result;
}

class Graph {
 public:
  const std::vector<Vertex>& get_vertices() const { return vertices_; }
  const std::vector<Edge>& get_edges() const { return edges_; }

  const Vertex& get_vertex(const VertexId& id) const {
    for (const Vertex& vertex : vertices_) {
      if (vertex.id == id) {
        return vertex;
      }
    }
    throw std::runtime_error("Can't get vertex by id");
  }

  Vertex& get_vertex(const VertexId& id) {
    const auto& const_this = *this;
    return const_cast<Vertex&>(const_this.get_vertex(id));
  }

  VertexId add_new_vertex() {
    if (vertices_.size() == 0) {
      depths_map_.emplace_back();
    }
    const VertexId new_id = get_new_vertex_id();
    vertices_.emplace_back(new_id);
    depths_map_[0].push_back(new_id);
    return new_id;
  }

  bool check_vertex_existence(const VertexId& vertex_id) const {
    for (const auto& vertex : vertices_) {
      if (vertex_id == vertex.id) {
        return true;
      }
    }
    return false;
  }

  bool are_vertices_connected(const VertexId& id1, const VertexId& id2) const {
    assert(check_vertex_existence(id1) &&
           "Attemptig to access to nonexistent vertex: Error.");
    assert(check_vertex_existence(id2) &&
           "Attemptig to access to nonexistent vertex: Error.");
    if (connections_map_.find(id1) == connections_map_.end() ||
        connections_map_.find(id2) == connections_map_.end()) {
      return false;
    }
    for (const auto& edge_id : connections_map_.at(id1)) {
      if (((id1 == id2) &&
           (edges_[edge_id].vertex1_id == edges_[edge_id].vertex2_id)) ||
          ((id1 != id2) && (edges_[edge_id].vertex1_id == id2 ||
                            edges_[edge_id].vertex2_id == id2))) {
        return true;
      }
    }
    return false;
  }

  Edge::Color get_binding_color(const VertexId& id1,
                                const VertexId& id2) const {
    assert(check_vertex_existence(id1) &&
           "Attemptig to access to nonexistent vertex: Error.");
    assert(check_vertex_existence(id2) &&
           "Attemptig to access to nonexistent vertex: Error.");

    if (id1 == id2) {
      return Edge::Color::Green;
    }

    const auto& vertex1 = get_vertex(id1);
    const auto& vertex2 = get_vertex(id2);

    if (vertex1.get_edge_ids().size() == 0 ||
        vertex2.get_edge_ids().size() == 0) {
      return Edge::Color::Grey;
    } else if (vertex1.depth - vertex2.depth == 0) {
      return Edge::Color::Blue;
    } else if (vertex1.depth - vertex2.depth == 1 ||
               vertex1.depth - vertex2.depth == -1) {
      return Edge::Color::Yellow;
    } else if (vertex1.depth - vertex2.depth == 2 ||
               vertex1.depth - vertex2.depth == -2) {
      return Edge::Color::Red;
    }

    throw std::runtime_error("Can't get binding color");
  }

  void bind_vertices(const VertexId& id1, const VertexId& id2) {
    assert(check_vertex_existence(id1) &&
           "Attemptig to access to nonexistent vertex: Error.");
    assert(check_vertex_existence(id2) &&
           "Attemptig to access to nonexistent vertex: Error.");
    assert(!are_vertices_connected(id1, id2) &&
           "Attemptig to connect connected vertices: Error.");
    Edge::Color edge_color = get_binding_color(id1, id2);

    if (edge_color == Edge::Color::Grey) {
      VertexId to_id = id1;
      VertexId from_id = id2;
      if (get_vertex(id2).get_edge_ids().size() == 0) {
        VertexId temp = from_id;
        from_id = to_id;
        to_id = temp;
      }
      get_vertex(to_id).depth = get_vertex(from_id).depth + 1;
      if (depths_map_.size() - 1 < get_vertex(to_id).depth) {
        depths_map_.emplace_back();
      }
      depths_map_[0].erase(
          std::remove(depths_map_[0].begin(), depths_map_[0].end(), to_id),
          depths_map_[0].end());
      depths_map_[depths_map_.size() - 1].push_back(to_id);
    }

    const auto& edge =
        edges_.emplace_back(get_new_edge_id(), id1, id2, edge_color);
    connections_map_[id1].push_back(edge.id);
    get_vertex(id1).add_edge_id(edge.id);
    if (id1 != id2) {
      connections_map_[id2].push_back(edge.id);
      get_vertex(id2).add_edge_id(edge.id);
    }
    colors_map_[edge_color].push_back(edge.id);
  }

  operator std::string() const {
    std::string result = "{\n  \"vertices\": [\n";

    for (int i = 0; i < vertices_.size(); i++) {
      result += std::string(vertices_[i]);
      if (i != vertices_.size() - 1)
        result += ",\n";
    }

    result += "\n  ],\n  \"edges\": [\n";

    for (int i = 0; i < edges_.size(); i++) {
      result += std::string(edges_[i]);
      if (i != edges_.size() - 1)
        result += ",\n";
    }

    result += "\n  ]\n}";
    return result;
  }

  std::vector<std::vector<VertexId>> depths_map_;

 private:
  std::vector<Edge> edges_;
  std::vector<Vertex> vertices_;

  VertexId vertex_id_counter_ = 0;
  EdgeId edge_id_counter_ = 0;

  // connections_map: vertex1 -> edge -> vertex2
  std::map<VertexId, std::vector<EdgeId>> connections_map_;
  std::map<Edge::Color, std::vector<EdgeId>> colors_map_;

  VertexId get_new_vertex_id() { return vertex_id_counter_++; }
  EdgeId get_new_edge_id() { return edge_id_counter_++; }
};

class GraphGenerator {
 public:
  struct Params {
    explicit Params(int init_depth = 0, int init_new_vertices_num = 0)
        : depth(init_depth), new_vertices_num(init_new_vertices_num) {}

    const int depth = 0;
    const int new_vertices_num = 0;
  };

  explicit GraphGenerator(const Params& params = Params()) : params_(params) {}

  Graph generate_random_graph() const {
    Graph graph = Graph();
    graph.add_new_vertex();

    generate_grey_edges(graph, params_.depth, params_.new_vertices_num);
    generate_green_edges(graph);
    generate_blue_edges(graph);
    generate_yellow_edges(graph);
    generate_red_edges(graph);

    return graph;
  }

 private:
  const Params params_ = Params();

  bool random_bool(float true_prob) const {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::bernoulli_distribution d(true_prob);
    return d(gen);
  }

  VertexId random_vertex_id(const std::vector<VertexId>& vertex_ids) const {
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> random_index(
        0, vertex_ids.size() - 1);
    return vertex_ids[random_index(rng)];
  }

  void generate_grey_edges(Graph& graph,
                           int depth,
                           int new_vertices_num) const {
    const float probability_decreasement = 1.0 / depth;
    float new_vertex_prob = 1.0;
    for (int cur_depth = 0; cur_depth < depth; cur_depth++) {
      if (graph.depths_map_.size() - 1 < cur_depth) {
        return;
      }
      const auto& vertex_ids_at_depth = graph.depths_map_[cur_depth];
      for (const VertexId cur_vertex_id : vertex_ids_at_depth) {
        for (int k = 0; k < new_vertices_num; k++) {
          if (random_bool(new_vertex_prob)) {
            const auto new_vertex_id = graph.add_new_vertex();
            graph.bind_vertices(cur_vertex_id, new_vertex_id);
          }
        }
      }
      new_vertex_prob -= probability_decreasement;
    }
  }

  void generate_green_edges(Graph& graph) const {
    for (const auto& cur_vertex : graph.get_vertices()) {
      if (random_bool(GREEN_PROB)) {
        graph.bind_vertices(cur_vertex.id, cur_vertex.id);
      }
    }
  }

  void generate_blue_edges(Graph& graph) const {
    for (int cur_depth = 0; cur_depth < graph.depths_map_.size(); cur_depth++) {
      const auto& vertex_ids_at_depth = graph.depths_map_[cur_depth];
      const int last_id = vertex_ids_at_depth[vertex_ids_at_depth.size() - 1];
      for (const VertexId cur_id : vertex_ids_at_depth) {
        if (cur_id != last_id && random_bool(BLUE_PROB)) {
          graph.bind_vertices(cur_id, cur_id + 1);
        }
      }
    }
  }

  void generate_yellow_edges(Graph& graph) const {
    float yellow_probability = 0;
    const float probability_increasement = 1.0 / (graph.depths_map_.size() - 1);
    for (int cur_depth = 0; cur_depth < graph.depths_map_.size() - 1;
         cur_depth++) {
      const auto& vertex_ids_at_depth = graph.depths_map_[cur_depth];
      const auto& vertex_ids_at_next_depth = graph.depths_map_[cur_depth + 1];
      for (const VertexId cur_id : vertex_ids_at_depth) {
        if (random_bool(yellow_probability)) {
          std::vector<VertexId> possible_connections;
          for (const VertexId next_id : vertex_ids_at_next_depth) {
            if (!graph.are_vertices_connected(cur_id, next_id)) {
              possible_connections.push_back(next_id);
            }
          }
          if (possible_connections.size() > 0) {
            const VertexId binding_id = random_vertex_id(possible_connections);
            graph.bind_vertices(cur_id, binding_id);
          }
        }
      }
      yellow_probability += probability_increasement;
    }
  }

  void generate_red_edges(Graph& graph) const {
    for (int cur_depth = 0; cur_depth < graph.depths_map_.size() - 2;
         cur_depth++) {
      const auto& vertex_ids_at_depth = graph.depths_map_[cur_depth];
      for (const VertexId cur_id : vertex_ids_at_depth) {
        if (random_bool(RED_PROB)) {
          const VertexId binding_id =
              random_vertex_id(graph.depths_map_[cur_depth + 2]);
          graph.bind_vertices(cur_id, binding_id);
        }
      }
    }
  }
};

int main() {
  int depth = 0, new_vertices_num = 0;

  std::cout << "Enter the depth:" << std::endl;
  std::cin >> depth;
  while (depth < 0) {
    std::cout << "Depth can't be negative" << std::endl;
    std::cout << "Enter the depth:" << std::endl;
    std::cin >> depth;
  }

  std::cout << "Enter max amount of vertices genereted from one vertex:"
            << std::endl;
  std::cin >> new_vertices_num;
  while (new_vertices_num < 0) {
    std::cout
        << "Max amount of vertices genereted from one vertex can't be negative"
        << std::endl;
    std::cout << "Enter max amount of vertices genereted from one vertex:"
              << std::endl;
    std::cin >> new_vertices_num;
  }

  const GraphGenerator::Params params(depth, new_vertices_num);
  const GraphGenerator generator(params);

  const Graph graph = generator.generate_random_graph();

  std::ofstream file;
  file.open("graph.json");

  if (file.is_open()) {
    file << std::string(graph) << std::endl;
    file.close();
  }

  return 0;
}
