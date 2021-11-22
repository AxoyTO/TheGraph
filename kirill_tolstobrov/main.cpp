#include <cassert>
#include <chrono>
#include <fstream>
#include <iostream>
#include <iterator>
#include <map>
#include <vector>

using VertexId = int;
using EdgeId = int;

enum class Color { Grey, Green, Blue, Yellow, Red };

class Vertex {
 public:
  explicit Vertex(const VertexId& init_id = 0) : id(init_id), depth(0) {}
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
  int depth;

 private:
  std::vector<EdgeId> edge_ids_;
};

class Edge {
 public:
  Edge(const EdgeId& init_id,
       const VertexId& v1_init,
       const VertexId& v2_init,
       const Color& color_init)
      : id(init_id),
        vertex1_id(v1_init),
        vertex2_id(v2_init),
        color(color_init){};
  operator std::string() const {
    std::string result = "";
    result += "{\n\"id\": " + std::to_string(id);
    result += ",\n\"vertex_ids\": ";

    result += "[" + std::to_string(vertex1_id);
    result += ", " + std::to_string(vertex2_id);
    result += "],\n\"color\": \"";
    switch (color) {
      case Color::Grey:
        result += "grey";
        break;
      case Color::Green:
        result += "green";
        break;
      case Color::Blue:
        result += "blue";
        break;
      case Color::Yellow:
        result += "yellow";
        break;
      case Color::Red:
        result += "red";
        break;
    }
    result += "\"\n}";

    return result;
  }

  const EdgeId id;
  const VertexId vertex1_id;
  const VertexId vertex2_id;
  Color color;
};

class Graph {
 public:
  Graph() : depth(0) {
    colors_counter[Color::Grey] = 0;
    colors_counter[Color::Green] = 0;
    colors_counter[Color::Blue] = 0;
    colors_counter[Color::Yellow] = 0;
    colors_counter[Color::Red] = 0;
  }
  int get_vertices_amount() const { return vertices_.size(); }
  int get_edges_amount() const { return edges_.size(); }

  void add_new_vertex() { vertices_.emplace_back(get_new_vertex_id()); }

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
    if (connections_map_.find(id1) == connections_map_.end()) {
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

  void bind_vertices(const VertexId& id1,
                     const VertexId& id2,
                     Color edge_color = Color::Grey) {
    bool vertices_connected_flag = are_vertices_connected(id1, id2);
    if (edge_color == Color::Yellow && vertices_connected_flag) {
      return;
    }
    assert(check_vertex_existence(id1) &&
           "Attemptig to access to nonexistent vertex: Error.");
    assert(check_vertex_existence(id2) &&
           "Attemptig to access to nonexistent vertex: Error.");
    assert(!vertices_connected_flag &&
           "Attemptig to connect connected vertices: Error.");

    if (edge_color == Color::Grey) {
      vertices_[id2].depth = vertices_[id1].depth + 1;
      if (depth < vertices_[id2].depth) {
        depth = vertices_[id2].depth;
      }
    }

    const auto& edge =
        edges_.emplace_back(get_new_edge_id(), id1, id2, edge_color);
    connections_map_[id1].push_back(edge.id);
    connections_map_[id2].push_back(edge.id);
    vertices_[id1].add_edge_id(edge.id);
    if (id1 != id2)
      vertices_[id2].add_edge_id(edge.id);
    colors_counter[edge_color]++;
  }

  void set_depths_map(std::vector<std::pair<int, int>>& d_m) {
    depths_map = d_m;
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

  int depth;

 private:
  std::vector<Edge> edges_;
  std::vector<Vertex> vertices_;

  VertexId vertex_id_counter_ = 0;
  EdgeId edge_id_counter_ = 0;

  // connections_map: vertex1 -> edge -> vertex2
  std::map<VertexId, std::vector<EdgeId>> connections_map_;
  std::vector<std::pair<int, int>> depths_map;
  std::map<Color, int> colors_counter;

  VertexId get_new_vertex_id() { return vertex_id_counter_++; }
  EdgeId get_new_edge_id() { return edge_id_counter_++; }
};

double random_prob() {
  return (double)std::rand() / RAND_MAX;
}

int generate_grey_edges(Graph& graph,
                        int depth,
                        int new_vertices_num,
                        std::vector<std::pair<int, int>>& depths_map,
                        const float probability_decreasement) {
  int vertices_amount = 1;
  float new_vertex_prob = 1.0;
  for (int i = 0; i < depth; i++) {
    for (int j = depths_map[i].first; j <= depths_map[i].second; j++) {
      for (int k = 0; k < new_vertices_num; k++) {
        if (random_prob() <= new_vertex_prob) {
          graph.add_new_vertex();
          vertices_amount++;
          graph.bind_vertices(j, vertices_amount - 1);
          if (depths_map.size() <= i + 1) {
            depths_map.push_back({vertices_amount - 1, vertices_amount - 1});
          } else {
            depths_map[i + 1].second++;
          }
        }
      }
    }

    new_vertex_prob -= probability_decreasement;
  }
  return vertices_amount;
}

void generate_yellow_edge(Graph& graph,
                          std::vector<std::pair<int, int>>& depths_map,
                          int cur_depth,
                          int cur_id) {
  int next_depth_start = depths_map[cur_depth + 1].first;
  int next_depth_finish = depths_map[cur_depth + 1].second;

  int binded_id = next_depth_start +
                  (std::rand() % (next_depth_finish - next_depth_start + 1));

  graph.bind_vertices(cur_id, binded_id, Color::Yellow);
}

void generate_red_edge(Graph& graph,
                       std::vector<std::pair<int, int>>& depths_map,
                       int cur_depth,
                       int cur_id) {
  int next_next_depth_start = depths_map[cur_depth + 2].first;
  int next_next_depth_finish = depths_map[cur_depth + 2].second;

  int binded_id =
      next_next_depth_start +
      (std::rand() % (next_next_depth_finish - next_next_depth_start + 1));

  graph.bind_vertices(cur_id, binded_id, Color::Red);
}

Graph generate_random_graph(int depth, int new_vertices_num) {
  Graph graph = Graph();

  srand(std::time(NULL));

  const float probability_decreasement = 1.0 / depth;
  graph.add_new_vertex();
  // depths_map: depth -> (first_vertex_id; last_vertex_id)
  std::vector<std::pair<int, int>> depths_map;
  depths_map.push_back({0, 0});

  int vertices_amount = generate_grey_edges(
      graph, depth, new_vertices_num, depths_map, probability_decreasement);

  enum { GREEN_PROB = 10, BLUE_PROB = 25, RED_PROB = 33, PROB_COEF = 100 };
  int cur_depth = 0;
  float yellow_probability = 0;
  const float probability_increasement = probability_decreasement;

  for (int cur_id = 0; cur_id < vertices_amount; cur_id++) {
    if (random_prob() <= GREEN_PROB / (float)PROB_COEF) {
      graph.bind_vertices(cur_id, cur_id, Color::Green);
    }

    if (cur_id != depths_map[cur_depth].second) {
      if (random_prob() <= BLUE_PROB / (float)PROB_COEF) {
        graph.bind_vertices(cur_id, cur_id + 1, Color::Blue);
      }
    }

    if (cur_depth != depth &&
        random_prob() <= yellow_probability * 100 / (float)PROB_COEF) {
      generate_yellow_edge(graph, depths_map, cur_depth, cur_id);
    }

    if (cur_depth < (depth - 1) &&
        random_prob() <= RED_PROB / (float)PROB_COEF) {
      generate_red_edge(graph, depths_map, cur_depth, cur_id);
    }

    if (cur_id == depths_map[cur_depth].second) {
      cur_depth++;
      yellow_probability += probability_increasement;
    }
  }

  graph.set_depths_map(depths_map);
  return graph;
}

int main() {
  int depth = 0, new_vertices_num = 0;

  std::cout << "Enter the depth:" << std::endl;
  std::cin >> depth;
  assert(depth >= 0 && "Depth can't be negative");

  std::cout << "Enter max amount of vertices genereted from one vertex:"
            << std::endl;
  std::cin >> new_vertices_num;
  assert(new_vertices_num >= 0 &&
         "Max amount of vertices genereted from one vertex can't be negative");

  Graph graph = generate_random_graph(depth, new_vertices_num);

  std::ofstream file;
  file.open("graph.json");

  if (file.is_open()) {
    file << std::string(graph) << std::endl;
    file.close();
  }

  return 0;
}
