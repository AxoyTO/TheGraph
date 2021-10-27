#include <assert.h>
#include <stdlib.h>
#include <time.h>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

constexpr int INVALID_ID = -1;

using std::pair;
using std::vector;
using EdgeId = int;
using VertexId = int;
using std::string;

enum class Color { Gray, Green, Red, Blue, Yellow };

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

class Graph {
 public:
  class Edge {
   public:
    Edge(const pair<VertexId, VertexId>& new_vertex_ids,
         const EdgeId& edge_id,
         const Color& new_color = Color::Gray)
        : vertex_ids(new_vertex_ids), id(edge_id), color(new_color) {}

    std::string to_string() const {
      std::stringstream buffer;
      buffer << "{\"id\":" << id << ",\"vertex_ids\":[" << vertex_ids.first
             << "," << vertex_ids.second << "],\"color\":\""
             << color_to_string(color) << "\"}";
      return buffer.str();
    }
    const pair<VertexId, VertexId>& get_vertex_ids() const {
      return vertex_ids;
    }
    const EdgeId id = 0;
    const pair<VertexId, VertexId> vertex_ids = {-1, -1};
    const Color color = Color::Gray;
  };

  class Vertex {
   public:
    explicit Vertex(const VertexId& new_id, int new_depth)
        : id(new_id), depth(new_depth) {}

    std::string to_string() const {
      std::stringstream buffer;
      buffer << "{\"id\":" << id << ",\"edge_ids\":[";
      for (int i = 0; i < edge_ids_.size() - 1; i++)
        buffer << edge_ids_[i] << ",";
      buffer << edge_ids_[edge_ids_.size() - 1] << "],\"depth\":" << depth
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
    const VertexId id = INVALID_ID;
    const int depth = 0;

   private:
    vector<EdgeId> edge_ids_ = {};
  };

  bool vertex_exist(const VertexId& id) const {
    for (const auto& vertex : vertices_)
      if (vertex.id == id)
        return true;
    return false;
  }

  bool edge_exist(const VertexId& first, const VertexId& second) const {
    for (const auto& edge_id_from_first_vertex :
         vertices_[first].get_edge_ids())
      for (const auto& edge_id_from_second_vertex :
           vertices_[second].get_edge_ids())
        if (edge_id_from_first_vertex == edge_id_from_second_vertex)
          return true;
    return false;
  }

  VertexId add_vertex(const int& depth) {
    VertexId vertex_id = get_new_vertex_id();
    vertices_.emplace_back(vertex_id, depth);
    if (depth > depth_)
      depth_ = depth;
    return vertex_id;
  }

  void add_edge(const VertexId& first,
                const VertexId& second,
                const Color& color = Color::Gray) {
    assert(vertex_exist(first) && "Source vertex id doesn't exist");
    assert(vertex_exist(second) && "Destination vertex id doesn't exist");
    std::cout << "id: " << first << " " << second << std::endl;
    if (first != second)
      assert(!edge_exist(first, second) && "Such edge already exist");
    const auto& new_edge = edges_.emplace_back(
        pair<VertexId, VertexId>{first, second}, get_new_edge_id(), color);
    vertices_[first].add_edge_id(new_edge.id);
    if (first != second)
      vertices_[second].add_edge_id(new_edge.id);
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

class Graph_generator {
 public:
  void set_params(const int& depth = 0, const int& new_vertices_num = 0) {
    if (depth)
      depth_ = depth;
    if (new_vertices_num)
      new_vertices_num_ = new_vertices_num;
  }

  // void set_graph(const Graph& graph) { graph_ = graph; }

  const Graph& get_graph() const { return graph_; }

  VertexId random_vertex_generator(int depth) {
    const int luck = rand() % depth_;
    // std::cout << luck << "_" << depth_ - depth - 1 << "|| ";
    if (luck < depth_ - depth)
      return graph_.add_vertex(depth);
    else
      return INVALID_ID;
  }

  bool random_colour_edge_generator(const Color& color, int depth = 0) const {
    int luck = 0;
    switch (color) {
      case Color::Green:
        luck = rand() % 10;
        return (luck == 0);
      case Color::Red:
        luck = rand() % 3;
        return (luck == 0);
      case Color::Blue:
        luck = rand() % 4;
        return (luck == 0);
      case Color::Yellow:
        luck = rand() % depth_;
        return (depth > luck);
      default:
        break;
    }
    return false;
  }

  void generate() {
    srand(time(0));
    vector<vector<VertexId> > levels = {{graph_.add_vertex(0)}};
    for (int i = 1; i <= depth_; i++) {
      levels.emplace_back(0);
      for (const auto& vertex_id : levels[i - 1])
        for (int j = 0; j < new_vertices_num_; j++) {
          VertexId new_vertex_id = random_vertex_generator(i - 1);
          if (new_vertex_id != INVALID_ID) {
            levels[i].push_back(new_vertex_id);
            graph_.add_edge(vertex_id, new_vertex_id);
          }
        }
    }
    for (int i = 1; i < depth_; i++) {
      vector<VertexId> unmarked_blue = levels[i];
      for (int j = 0; j < levels[i].size(); j++) {
        std::cout << levels[i][j] << " ";
        if (random_colour_edge_generator(Color::Green)) {
          graph_.add_edge(levels[i][j], levels[i][j], Color::Green);
        }
        if (j > 0 && j < levels[i].size() - 1 &&
            random_colour_edge_generator(Color::Blue)) {
          bool randomiser = rand() % 2;
          VertexId nearest_id =
              randomiser ? levels[i][j - 1] : levels[i][j + 1];
          if (!graph_.edge_exist(levels[i][j], nearest_id))
            graph_.add_edge(levels[i][j], nearest_id, Color::Blue);
        }
        if (i < levels.size() - 2 && levels[i + 2].size() > 0 &&
            random_colour_edge_generator(Color::Red)) {
          graph_.add_edge(levels[i][j],
                          levels[i + 2][rand() % levels[i + 2].size()],
                          Color::Red);
        }
        if (i < levels.size() - 1 &&
            random_colour_edge_generator(Color::Yellow, i)) {
          vector<VertexId> new_vertices = {};
          for (const auto& vertex_id : levels[i + 1])
            if (!graph_.edge_exist(levels[i][j], vertex_id))
              new_vertices.push_back(vertex_id);
          if (new_vertices.size() > 0)
            graph_.add_edge(levels[i][j],
                            new_vertices[rand() % new_vertices.size()],
                            Color::Yellow);
        }
      }
      std::cout << std::endl;
    }
  }

 private:
  int depth_ = 0;
  int new_vertices_num_ = 0;
  Graph graph_;
};

int main() {
  int depth = 0, new_vertices_num = 0;
  std::cin >> depth >> new_vertices_num;
  Graph_generator generator{};
  generator.set_params(depth, new_vertices_num);
  generator.generate();
  // Graph graph = generator.get_graph();
  std::ofstream file;
  file.open("graph.json", std::fstream::out | std::fstream::trunc);
  // file << graph.to_json();
  file << generator.get_graph().to_json();
  file.close();
  return 0;
}
