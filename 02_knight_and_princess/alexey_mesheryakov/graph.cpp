#include <assert.h>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iostream>
#include <random>
#include <sstream>
#include <string>
#include <vector>

constexpr int INVALID_ID = -1;
const double EPS = 1e-9;

using std::pair;
using std::vector;
using EdgeId = int;
using VertexId = int;
using std::string;

bool is_lucky(const double probability) {
  assert(probability + EPS > 0 && probability - EPS < 1 &&
         "given probability is incorrect");
  static std::knuth_b rand_engine{};
  std::mt19937 rng{rand_engine()};
  std::bernoulli_distribution bernoullu_distribution_var(probability);
  return bernoullu_distribution_var(rng);
}

class Graph {
 public:
  class Edge {
   public:
    enum class Color { Gray, Green, Red, Blue, Yellow };
    Edge(const pair<VertexId, VertexId>& new_vertex_ids,
         const EdgeId& edge_id,
         const Color& new_color = Color::Gray)
        : vertex_ids_(new_vertex_ids), id_(edge_id), color_(new_color) {}

    std::string to_string() const;
    const pair<VertexId, VertexId>& get_vertex_ids() const {
      return vertex_ids_;
    }
    const EdgeId& get_id() const { return id_; }

   private:
    EdgeId id_ = 0;
    pair<VertexId, VertexId> vertex_ids_ = {INVALID_ID, INVALID_ID};
    Color color_ = Color::Gray;
  };
  using Color = Edge::Color;
  class Vertex {
   public:
    explicit Vertex(const VertexId& new_id) : id_(new_id) {}

    std::string to_string() const {
      std::stringstream buffer;
      buffer << "{\"id\":" << id_ << ",\"edge_ids\":[";
      for (int i = 0; i < edge_ids_.size() - 1; i++)
        buffer << edge_ids_[i] << ",";
      buffer << edge_ids_[edge_ids_.size() - 1] << "],\"depth\":" << depth_
             << "}";
      return buffer.str();
    }

    void add_edge_id(const EdgeId& edge_id) {
      assert(!has_edge_id(edge_id) && "Edge id already exist");
      edge_ids_.push_back(edge_id);
    }

    bool has_edge_id(const EdgeId& new_edge_id) const {
      for (const auto& edge_id : edge_ids_)
        if (edge_id == new_edge_id) {
          return true;
        }
      return false;
    }
    const vector<EdgeId>& get_edge_ids() const { return edge_ids_; }
    const VertexId& get_id() const { return id_; }
    void set_depth(int depth) { depth_ = depth; }
    int get_depth() const { return (depth_); }

   private:
    vector<EdgeId> edge_ids_ = {};
    VertexId id_ = INVALID_ID;
    int depth_ = 0;
  };

  bool vertex_exist(const VertexId& id) const {
    for (const auto& vertex : vertices_)
      if (vertex.get_id() == id)
        return true;
    return false;
  }

  bool edge_exist(const VertexId& first, const VertexId& second) const {
    if (first == second)
      return false;
    for (const auto& edge_id_from_first_vertex :
         vertices_[first].get_edge_ids())
      for (const auto& edge_id_from_second_vertex :
           vertices_[second].get_edge_ids())
        if (edge_id_from_first_vertex == edge_id_from_second_vertex)
          return true;
    return false;
  }

  VertexId add_vertex() {
    VertexId vertex_id = get_new_vertex_id();
    vertices_.emplace_back(vertex_id);
    return vertex_id;
  }

  void add_edge(const VertexId& first_id,
                const VertexId& second_id,
                const Color& color = Color::Gray) {
    VertexId first = std::min(first_id, second_id);
    VertexId second = std::max(first_id, second_id);
    assert(vertex_exist(first) && "Source vertex id doesn't exist");
    assert(vertex_exist(second) && "Destination vertex id doesn't exist");
    std::cout << "id: " << first << " " << second << std::endl;
    assert(!edge_exist(first, second) && "Such edge already exist");
    const auto& new_edge = edges_.emplace_back(
        pair<VertexId, VertexId>{first, second}, get_new_edge_id(), color);
    vertices_[first].add_edge_id(new_edge.get_id());
    if (first != second) {
      vertices_[second].add_edge_id(new_edge.get_id());
      if (color == Edge::Color::Gray) {
        int new_depth = vertices_[first].get_depth() + 1;
        if (depth_ < new_depth)
          depth_ = new_depth;
        vertices_[second].set_depth(new_depth);
      }
    }
  }

  std::string to_json() const {
    std::stringstream buffer;
    buffer << "{\"depth\":" << depth_ << ",\"vertices\":[";
    for (int j = 0; j < vertices_.size(); j++) {
      Vertex vertex = vertices_[j];
      buffer << vertex.to_string();
      if (j != vertices_.size() - 1)
        buffer << ",";
    }
    buffer << "],\"edges\":[";
    for (int j = 0; j < edges_.size(); j++) {
      Edge edge = edges_[j];
      buffer << edge.to_string();
      if (j != edges_.size() - 1)
        buffer << ",";
    }
    buffer << "]}\n";
    return buffer.str();
  }

 private:
  vector<Edge> edges_ = {};
  vector<Vertex> vertices_ = {};
  VertexId vertex_id_counter_ = 0;
  EdgeId edge_id_counter_ = 0;

  EdgeId get_new_edge_id() { return edge_id_counter_++; }

  VertexId get_new_vertex_id() { return vertex_id_counter_++; }
  int depth_ = 0;
};

using Color = Graph::Edge::Color;

std::string color_to_string(const Color& color) {
  switch (color) {
    case Color::Gray:
      return "gray";
    case Color::Green:
      return "green";
    case Color::Red:
      return "red";
    case Color::Blue:
      return "blue";
    case Color::Yellow:
      return "yellow";
  }
}

std::string Graph::Edge::to_string() const {
  std::stringstream buffer;
  buffer << "{\"id\":" << id_ << ",\"vertex_ids\":[" << vertex_ids_.first << ","
         << vertex_ids_.second << "],\"color\":\"" << color_to_string(color_)
         << "\"}";
  return buffer.str();
}

class Graph_Generator {
 public:
  Graph_Generator(int depth = 0, int new_vertices_num = 0) {
    if (depth)
      depth_ = depth;
    if (new_vertices_num)
      new_vertices_num_ = new_vertices_num;
  }

  VertexId random_vertex_generator(int depth, Graph& graph) const {
    // std::cout << luck << "_" << depth_ - depth - 1 << "|| ";
    if (is_lucky(1.0 - (double)depth / depth_))
      return graph.add_vertex();
    else
      return INVALID_ID;
  }

  bool random_colour_edge_generator(const Color& color, int depth = 0) const {
    int luck = 0;
    switch (color) {
      case Color::Green:
        return is_lucky(0.1);
      case Color::Red:
        return is_lucky(0.333);
      case Color::Blue:
        return is_lucky(0.25);
      case Color::Yellow:
        return is_lucky((double)depth / depth_);
      default:
        break;
    }
    return false;
  }

  void generate_yellow_edge(Graph& graph,
                            const vector<vector<VertexId>>& levels,
                            int i,
                            int j) const {
    if (i < levels.size() - 1 &&
        random_colour_edge_generator(Color::Yellow, i)) {
      vector<VertexId> new_vertices = {};
      for (const auto& vertex_id : levels[i + 1])
        if (!graph.edge_exist(levels[i][j], vertex_id))
          new_vertices.push_back(vertex_id);
      // std::cout<<std::rand() % new_vertices.size();
      if (new_vertices.size() > 0)
        graph.add_edge(levels[i][j],
                       new_vertices[std::rand() % new_vertices.size()],
                       Color::Yellow);
    }
  }

  void generate_red_edge(Graph& graph,
                         const vector<vector<VertexId>>& levels,
                         int i,
                         int j) const {
    if (i < levels.size() - 2 && levels[i + 2].size() > 0 &&
        random_colour_edge_generator(Color::Red)) {
      graph.add_edge(levels[i][j],
                     levels[i + 2][std::rand() % levels[i + 2].size()],
                     Color::Red);
    }
  }

  void generate_blue_edge(Graph& graph,
                          const vector<vector<VertexId>>& levels,
                          int i,
                          int j) const {
    if (j > 0 && j < levels[i].size() - 1 &&
        random_colour_edge_generator(Color::Blue)) {
      VertexId nearest_id = is_lucky(0.5) ? levels[i][j - 1] : levels[i][j + 1];
      if (!graph.edge_exist(levels[i][j], nearest_id))
        graph.add_edge(levels[i][j], nearest_id, Color::Blue);
    }
  }

  void generate_green_edge(Graph& graph,
                           const vector<vector<VertexId>>& levels,
                           int i,
                           int j) const {
    if (random_colour_edge_generator(Color::Green)) {
      graph.add_edge(levels[i][j], levels[i][j], Color::Green);
    }
  }

  Graph generate() const {
    Graph graph;
    vector<vector<VertexId>> levels = {{graph.add_vertex()}};
    for (int i = 1; i <= depth_; i++) {
      levels.emplace_back(0);
      for (const auto& vertex_id : levels[i - 1])
        for (int j = 0; j < new_vertices_num_; j++) {
          VertexId new_vertex_id = random_vertex_generator(i - 1, graph);
          if (new_vertex_id != INVALID_ID) {
            levels[i].push_back(new_vertex_id);
            graph.add_edge(vertex_id, new_vertex_id);
          }
        }
    }
    for (int i = 1; i < depth_; i++) {
      for (int j = 0; j < levels[i].size(); j++) {
        std::cout << levels[i][j] << " ";
        generate_green_edge(graph, levels, i, j);
        generate_blue_edge(graph, levels, i, j);
        generate_red_edge(graph, levels, i, j);
        generate_yellow_edge(graph, levels, i, j);
      }
      std::cout << std::endl;
    }
    return graph;
  }

 private:
  int depth_ = 0;
  int new_vertices_num_ = 0;
};

int main() {
  std::srand(std::time(0));
  int depth = 0, new_vertices_num = 0;
  while ((depth <= 0) || (new_vertices_num <= 0)) {
    std::cin >> depth >> new_vertices_num;
    if (depth <= 0)
      std::cout << "Depth must be > 0, try again\n";
    if (new_vertices_num <= 0)
      std::cout << "Number of vertices must be > 0, try again\n";
  }
  Graph_Generator generator(depth, new_vertices_num);
  Graph graph = generator.generate();
  std::ofstream file;
  file.open("graph.json", std::fstream::out | std::fstream::trunc);
  file << graph.to_json();
  file.close();
  return 0;
}
