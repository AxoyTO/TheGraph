#include <algorithm>
#include <cassert>
#include <fstream>
#include <iostream>
#include <map>
#include <queue>
#include <random>
#include <set>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

const double EPS = 1e-9;

enum class Color { GRAY, GREEN, BLUE, YELLOW, RED };

using VertexId = int;
using EdgeId = int;

bool is_lucky(const double probability) {
  assert(probability + EPS > 0 && probability - EPS < 1 &&
         "given probability is incorrect");
  static std::knuth_b rand_engine{};
  std::mt19937 rng{rand_engine()};
  std::bernoulli_distribution bernoullu_distribution_var(probability);
  return bernoullu_distribution_var(rng);
}

class Vertex {
 public:
  const std::set<EdgeId>& connected_edges() const { return connected_edges_; }

  explicit Vertex(const VertexId& vertex_id) : id_(vertex_id) {}

  void init_depth(size_t new_depth) { depth = new_depth; }

  std::string get_json_string() const {
    std::stringstream json_stringstream;
    json_stringstream << "{\"id\":" << id_ << ","
                      << "\"edge_ids\":[";
    for (auto it = connected_edges_.begin(); it != connected_edges_.end();
         ++it) {
      json_stringstream << *it;
      if (std::next(it) != connected_edges_.end()) {
        json_stringstream << ",";
      }
    }
    json_stringstream << "],"
                      << "\"depth\":" << depth << "}";
    return json_stringstream.str();
  }

  bool has_edge_id(const EdgeId& edge_id) const {
    return connected_edges_.find(edge_id) != connected_edges_.end();
  }

  void add_edge(const EdgeId& edge_id) {
    assert(!has_edge_id(edge_id) && "edge that is to be added already exists");
    connected_edges_.insert(edge_id);
  }

 private:
  const VertexId id_;
  std::set<EdgeId> connected_edges_;
  size_t depth;
};

class Edge {
 public:
  const VertexId vertex1_id, vertex2_id;
  Edge(const EdgeId& edge_id,
       const VertexId& vertex1,
       const VertexId& vertex2,
       Color edge_color)
      : vertex1_id(vertex1),
        vertex2_id(vertex2),
        color_(edge_color),
        id_(edge_id) {}

  std::string get_json_string() const {
    static const std::map<Color, std::string> color_string_map = {
        {Color::GRAY, "gray"},
        {Color::GREEN, "green"},
        {Color::BLUE, "blue"},
        {Color::YELLOW, "yellow"},
        {Color::RED, "red"}};
    std::stringstream json_stringstream;
    std::string color_string = color_string_map.find(color_)->second;
    json_stringstream << "{\"id\":" << id_ << ","
                      << "\"vertex_ids\":[" << vertex1_id << "," << vertex2_id
                      << "],\"color\":\"" << color_string << "\""
                      << "}";
    return json_stringstream.str();
  }

 private:
  const Color color_;
  const EdgeId id_;
  const std::string color;
};

class Graph {
 public:
  size_t max_depth() const { return max_depth_; }

  std::set<VertexId> vertex_ids() const {
    std::set<VertexId> vertex_ids;
    std::transform(vertices_.begin(), vertices_.end(),
                   std::inserter(vertex_ids, vertex_ids.end()),
                   [](auto pair) { return pair.first; });
    return vertex_ids;
  }

  const std::set<VertexId>& get_vertices_at_depth(size_t depth) {
    update_vertices_depth_();
    return vertices_at_depth_[depth];
  }

  bool is_vertex_exists(const VertexId& vertex) const {
    return vertices_.find(vertex) != vertices_.end();
  }

  bool is_adjacent(const VertexId& vertex1, const VertexId& vertex2) const {
    return vertex1 + 1 == vertex2 || vertex2 + 1 == vertex1;
  }

  bool is_connected(const VertexId& vertex1, const VertexId& vertex2) const {
    assert(is_vertex_exists(vertex1) && "Vertex 1 doesn't exist");
    assert(is_vertex_exists(vertex2) && "Vertex 2 doesn't exist");
    if (vertex1 == vertex2) {
      auto it_vertex = vertices_.find(vertex1);
      for (const auto& vertex_edge_id : it_vertex->second.connected_edges()) {
        const auto& vertex_edge = edges_.find(vertex_edge_id)->second;
        if (vertex_edge.vertex1_id == vertex_edge.vertex2_id) {
          return true;
        }
      }
      return false;
    } else {
      auto it_vertex1 = vertices_.find(vertex1);
      for (const auto& vertex1_edge : it_vertex1->second.connected_edges()) {
        auto it_vertex2 = vertices_.find(vertex2);
        if (it_vertex2->second.has_edge_id(vertex1_edge)) {
          return true;
        }
      }
      return false;
    }
  }

  VertexId add_vertex() {
    const VertexId new_vertex_id = get_next_vertex_id_();
    vertices_.emplace(new_vertex_id, new_vertex_id);
    return new_vertex_id;
  }

  EdgeId add_edge(const VertexId& vertex1,
                  const VertexId& vertex2,
                  Color edge_color = Color::GRAY) {
    assert(is_vertex_exists(vertex1) && "Vertex 1 doesn't exist");
    assert(is_vertex_exists(vertex2) && "Vertex 2 doesn't exist");
    assert(!is_connected(vertex1, vertex2) && "Vertices already connected");

    const EdgeId new_edge_id = get_next_edge_id_();

    auto it_vertex1 = vertices_.find(vertex1);
    it_vertex1->second.add_edge(new_edge_id);
    auto it_vertex2 = vertices_.find(vertex2);
    if (it_vertex1 != it_vertex2) {
      it_vertex2->second.add_edge(new_edge_id);
    }

    edges_.emplace(new_edge_id,
                   Edge(new_edge_id, vertex1, vertex2, edge_color));
    return new_edge_id;
  }

  std::string get_json_string() {
    update_vertices_depth_();
    std::stringstream json_stringstream;
    json_stringstream << "{\"depth\":" << max_depth_ << ",";
    json_stringstream << "\"vertices\":[";
    for (auto it = vertices_.begin(); it != vertices_.end(); ++it) {
      json_stringstream << it->second.get_json_string();
      if (std::next(it) != vertices_.end()) {
        json_stringstream << ",";
      }
    }
    json_stringstream << "],\"edges\":[";
    for (auto it = edges_.begin(); it != edges_.end(); ++it) {
      json_stringstream << it->second.get_json_string();
      if (std::next(it) != edges_.end()) {
        json_stringstream << ",";
      }
    }
    json_stringstream << "]}";
    return json_stringstream.str();
  }

 private:
  VertexId next_vertex_id_{};
  EdgeId next_edge_id_{};
  std::map<VertexId, Vertex> vertices_;
  std::map<EdgeId, Edge> edges_;
  std::map<size_t, std::set<VertexId>> vertices_at_depth_;
  size_t max_depth_;

  VertexId get_next_vertex_id_() {
    VertexId new_vertex_id = next_vertex_id_;
    ++next_vertex_id_;
    return new_vertex_id;
  }

  EdgeId get_next_edge_id_() {
    EdgeId new_edge_id = next_edge_id_;
    ++next_edge_id_;
    return new_edge_id;
  }

  void update_vertices_depth_() {
    const VertexId first_vertex_id = vertices_.begin()->first;

    std::map<VertexId, size_t> depths;
    depths.emplace(first_vertex_id, 0);

    std::queue<VertexId> bfs_queue;
    bfs_queue.push(first_vertex_id);

    std::set<VertexId> used;
    used.insert(first_vertex_id);

    while (!bfs_queue.empty()) {
      const VertexId current_vertex_id = bfs_queue.front();
      bfs_queue.pop();
      for (const auto& connected_edge_id :
           vertices_.find(current_vertex_id)->second.connected_edges()) {
        const Edge connected_edge = edges_.find(connected_edge_id)->second;
        const VertexId vertex1_id = connected_edge.vertex1_id;
        const VertexId vertex2_id = connected_edge.vertex2_id;
        const VertexId connected_vertex_id =
            (current_vertex_id == vertex1_id ? vertex2_id : vertex2_id);
        if (used.find(connected_vertex_id) == used.end()) {
          used.insert(connected_vertex_id);
          depths[connected_vertex_id] = depths[current_vertex_id] + 1;
          bfs_queue.push(connected_vertex_id);
        }
      }
    }
    size_t new_max_depth = 0;
    for (const auto& [vertex_id, depth] : depths) {
      vertices_.find(vertex_id)->second.init_depth(depth);
      if (depth > new_max_depth) {
        new_max_depth = depth;
      }
    }
    max_depth_ = new_max_depth;
    for (const auto& [vertex_id, depth] : depths) {
      vertices_at_depth_[depth].insert(vertex_id);
    }
  }
};

Graph task_02_get_graph() {
  Graph task_02_graph;
  const size_t n = 14;
  std::vector<VertexId> added_vertices;
  for (size_t i = 0; i < n; ++i) {
    auto new_vertex = task_02_graph.add_vertex();
    added_vertices.push_back(new_vertex);
  }
  const std::vector<std::pair<size_t, size_t>> edge_vertices_indexes{
      {0, 1},  {0, 2},  {0, 3},  {1, 4},   {1, 5},   {1, 6},
      {2, 7},  {2, 8},  {3, 9},  {4, 10},  {5, 10},  {6, 10},
      {7, 11}, {8, 11}, {9, 12}, {10, 13}, {11, 13}, {12, 13}};
  for (const auto& edge : edge_vertices_indexes) {
    VertexId vertex1 = added_vertices[edge.first];
    VertexId vertex2 = added_vertices[edge.second];
    task_02_graph.add_edge(vertex1, vertex2);
  }
  return task_02_graph;
}

Graph task_03_get_graph(int depth, int new_vertices_num) {
  if (depth < 0) {
    throw std::invalid_argument("invalid depth argument");
  }
  if (new_vertices_num < 0) {
    throw std::invalid_argument("invalid new_vertices_num argument");
  }
  Graph working_graph;
  working_graph.add_vertex();
  for (int i = 0; i <= depth; ++i) {
    auto same_depth_vertices = working_graph.get_vertices_at_depth(i);
    for (const auto& current_vertex_id : same_depth_vertices) {
      for (int j = 0; j < new_vertices_num; ++j) {
        if (depth > 0 && is_lucky(1.0 - (double)i / depth)) {
          VertexId new_vertex = working_graph.add_vertex();
          working_graph.add_edge(current_vertex_id, new_vertex);
        }
      }
    }
  }
  // green edges:
  for (auto vertex_id : working_graph.vertex_ids()) {
    if (is_lucky(0.1)) {
      working_graph.add_edge(vertex_id, vertex_id, Color::GREEN);
    }
  }
  // blue edges:
  for (size_t cur_depth = 0; cur_depth < working_graph.max_depth();
       ++cur_depth) {
    const auto& same_depth_vertices =
        working_graph.get_vertices_at_depth(cur_depth);
    for (const auto& vertex1_id : same_depth_vertices) {
      for (const auto& vertex2_id : same_depth_vertices) {
        if (is_lucky(0.25) &&
            !working_graph.is_connected(vertex1_id, vertex2_id) &&
            working_graph.is_adjacent(vertex1_id, vertex2_id)) {
          working_graph.add_edge(vertex1_id, vertex2_id, Color::BLUE);
        }
      }
    }
  }
  // yellow edges:
  for (size_t cur_depth = 0; cur_depth + 1 <= working_graph.max_depth();
       ++cur_depth) {
    const auto& cur_depth_vertices =
        working_graph.get_vertices_at_depth(cur_depth);
    const auto& next_depth_vertices =
        working_graph.get_vertices_at_depth(cur_depth + 1);
    for (const auto& cur_vertex_id : cur_depth_vertices) {
      if (depth > 1 && is_lucky((double)cur_depth / (depth - 1))) {
        std::set<VertexId> not_connected_vertices;
        for (const auto& next_vertex_id : next_depth_vertices) {
          if (!working_graph.is_connected(cur_vertex_id, next_vertex_id)) {
            not_connected_vertices.insert(next_vertex_id);
          }
        }
        if (!not_connected_vertices.empty()) {
          size_t rand_id = std::rand() % not_connected_vertices.size();
          auto rand_it = not_connected_vertices.begin();
          std::advance(rand_it, rand_id);
          working_graph.add_edge(cur_vertex_id, *rand_it, Color::YELLOW);
        }
      }
    }
  }
  // red edges:
  for (size_t cur_depth = 0; cur_depth + 2 <= working_graph.max_depth();
       ++cur_depth) {
    const auto& cur_depth_vertices =
        working_graph.get_vertices_at_depth(cur_depth);
    const auto& next_depth_vertices =
        working_graph.get_vertices_at_depth(cur_depth + 2);
    for (const auto& cur_vertex_id : cur_depth_vertices) {
      if (is_lucky(0.33) && !next_depth_vertices.empty()) {
        size_t rand_id = std::rand() % next_depth_vertices.size();
        auto rand_it = next_depth_vertices.begin();
        std::advance(rand_it, rand_id);
        working_graph.add_edge(cur_vertex_id, *rand_it, Color::RED);
      }
    }
  }
  if (working_graph.max_depth() < (size_t)depth) {
    std::cerr << "generated graph's depth=" << working_graph.max_depth()
              << " is less than specified one =" << depth << std::endl;
  }
  return working_graph;
}

int main() {
  Graph task_03_graph;
  int depth;
  int new_vertices_num;
  while (true) {
    std::cout << "Input the depth parameter: " << std::flush;
    std::cin >> depth;
    std::cout << "Input the amount of new vertices: " << std::flush;
    std::cin >> new_vertices_num;
    try {
      task_03_graph = task_03_get_graph(depth, new_vertices_num);
      break;
    } catch (const std::invalid_argument& error) {
      std::cout << error.what() << std::endl
                << "Please, redo the input:" << std::endl;
    }
  }

  const std::string json_string = task_03_graph.get_json_string();
  std::fstream json_file;
  json_file.open("graph.json", std::ios::out);
  if (!json_file.is_open()) {
    throw std::runtime_error("file not created");
  }
  json_file << json_string << std::endl;
  json_file.close();
  return 0;
}
