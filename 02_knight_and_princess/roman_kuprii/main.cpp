#include <array>
#include <cassert>
#include <fstream>
#include <iostream>
#include <random>
#include <string>
#include <vector>

using std::endl;
using std::to_string;
using std::vector;

using EdgeId = int;
using VertexId = int;

constexpr int GRAPH_NUMBER = 10;
constexpr int START_VERTICES_NUMBER = 10;
constexpr int INVALID_ID = -1;
constexpr int INVALID_NUMBER = -1;
const std::string JSON_GRAPH_FILENAME = "temp/graph";
const std::string LOG_FILENAME = "log.txt";

int min(const int& a, const int& b) {
  return (a < b) ? a : b;
}

enum class Color { GRAY, GREEN, BLUE, YELLOW, RED };

struct Edge {
  const EdgeId id = INVALID_ID;
  const std::array<VertexId, 2> connected_vertices;
  Color color = Color::GRAY;

  explicit Edge(const VertexId& start, const VertexId& end, const EdgeId& _id)
      : id(_id), connected_vertices({start, end}) {}

  std::string to_json() const {
    std::string res;
    res = "{ \"id\": ";
    res += to_string(id);
    res += ", \"vertex_ids\": [";
    res += to_string(connected_vertices[0]);
    res += ", ";
    res += to_string(connected_vertices[1]);
    res += "], \"color\": ";
    switch (color) {
      case Color::GRAY: {
        res += "\"gray\" }";
        break;
      }
      case Color::GREEN: {
        res += "\"green\" }";
        break;
      }
      case Color::BLUE: {
        res += "\"blue\" }";
        break;
      }
      case Color::YELLOW: {
        res += "\"yellow\" }";
        break;
      }
      case Color::RED: {
        res += "\"red\" }";
        break;
      }
      default:
        break;
    }
    return res;
  }

  void paint(const Color& _color) { color = _color; }
};

bool is_edge_id_included(const EdgeId& id, const vector<EdgeId>& edge_ids) {
  for (const auto& edge_id : edge_ids)
    if (id == edge_id)
      return true;
  return false;
}

struct Vertex {
 public:
  const VertexId id = INVALID_ID;
  int depth = 0;

  explicit Vertex(const VertexId& _id) : id(_id) {}

  std::string to_json() const {
    std::string res;
    res = "{ \"id\": ";
    res += to_string(id) + ", \"edge_ids\": [";
    for (const auto& edge_id : edges_ids_) {
      res += to_string(edge_id);
      res += ", ";
    }
    res.pop_back();
    res.pop_back();
    res += "] }";
    return res;
  }

  void add_edge_id(const EdgeId& _id) {
    assert(!is_edge_id_included(_id, edges_ids_));
    edges_ids_.push_back(_id);
  }

  const vector<EdgeId>& get_edges_ids() const { return edges_ids_; }

 private:
  vector<EdgeId> edges_ids_;
};

class Graph {
 public:
  std::string to_json() const {
    std::string res;
    res = "{ \"depth\": ";
    res += to_string(depth_);
    res += ", \"vertices\": [ ";
    for (const auto& vertex : vertices_) {
      res += vertex.to_json();
      res += ", ";
    }
    res.pop_back();
    res.pop_back();
    res += " ], \"edges\": [ ";
    for (const auto& edge : edges_) {
      res += edge.to_json();
      res += ", ";
    }
    res.pop_back();
    res.pop_back();
    res += " ] }\n";
    return res;
  }

  void add_vertex() { vertices_.emplace_back(get_next_vertex_id()); }

  bool is_vertex_exist(const VertexId& vertex_id) const {
    for (const auto& vertex : vertices_) {
      if (vertex_id == vertex.id)
        return true;
    }
    return false;
  }

  bool is_connected(const VertexId& from_vertex_id,
                    const VertexId& to_vertex_id) const {
    assert(is_vertex_exist(from_vertex_id));
    assert(is_vertex_exist(to_vertex_id));

    const auto& from_vertex_edges_ids =
        vertices_[from_vertex_id].get_edges_ids();
    const auto& to_vertex_edges_ids = vertices_[to_vertex_id].get_edges_ids();
    for (const auto& from_vertex_edge_id : from_vertex_edges_ids) {
      for (const auto& to_vertex_edge_id : to_vertex_edges_ids) {
        if (from_vertex_edge_id == to_vertex_edge_id)
          return true;
      }
    }
    return false;
  }

  bool is_looped(const VertexId& vertex_id) const {
    const auto& vertex_edges_ids = vertices_[vertex_id].get_edges_ids();
    for (const auto& edge_id : vertex_edges_ids) {
      const auto& connected_vertices = edges_[edge_id].connected_vertices;
      if (connected_vertices[0] == connected_vertices[1])
        return true;
    }
    return false;
  }

  void connect_vertices(const VertexId& from_vertex_id,
                        const VertexId& to_vertex_id,
                        const bool& paint) {
    assert(is_vertex_exist(from_vertex_id));
    assert(is_vertex_exist(to_vertex_id));

    if (from_vertex_id != to_vertex_id)
      assert(!is_connected(from_vertex_id, to_vertex_id));
    else
      assert(!is_looped(from_vertex_id));

    const auto& new_edge =
        edges_.emplace_back(from_vertex_id, to_vertex_id, get_next_edge_id());

    // add information into Vertex structure
    vertices_[from_vertex_id].add_edge_id(new_edge.id);
    if (from_vertex_id != to_vertex_id)
      vertices_[to_vertex_id].add_edge_id(new_edge.id);

    if (!paint) {
      int min_depth = vertices_[from_vertex_id].depth;
      for (const auto& edge_idx : vertices_[to_vertex_id].get_edges_ids()) {
        VertexId vert = edges_[edge_idx].connected_vertices[0];
        min_depth = min(min_depth, vertices_[vert].depth);
      }
      vertices_[to_vertex_id].depth = min_depth + 1;

      if (depth_ < min_depth + 1)
        depth_ = min_depth + 1;
    }

    if (paint) {
      const int diff =
          vertices_[to_vertex_id].depth - vertices_[from_vertex_id].depth;
      if (from_vertex_id == to_vertex_id) {
        edges_[new_edge.id].color = Color::GREEN;
      } else if (diff == 0) {
        edges_[new_edge.id].color = Color::BLUE;
      } else if (diff == 1) {
        edges_[new_edge.id].color = Color::YELLOW;
      } else if (diff == 2) {
        edges_[new_edge.id].color = Color::RED;
      }
    }
  }

  vector<Edge> get_edges() const { return edges_; }
  vector<Vertex> get_vertices() const { return vertices_; }

  int get_depth() const { return depth_; }
  int get_vertices_num() const { return vertices_.size(); }

 private:
  vector<Vertex> vertices_;
  vector<Edge> edges_;
  int depth_ = 0;
  VertexId vertex_id_counter_ = 0;
  EdgeId edge_id_counter_ = 0;

  VertexId get_next_vertex_id() { return vertex_id_counter_++; }
  VertexId get_next_edge_id() { return edge_id_counter_++; }
};

void write_graph(const Graph& graph, const int& graph_number) {
  std::ofstream out;
  const std::string current_name =
      JSON_GRAPH_FILENAME + "_" + to_string(graph_number) + ".gson";
  out.open(current_name, std::ofstream::out | std::ofstream::trunc);
  out << graph.to_json();
  out.close();
}

void new_vertices_generation(Graph& work_graph,
                             const int& _depth,
                             const int& new_vertices_num) {
  const int graph_depth = work_graph.get_depth();
  int depth = min(graph_depth, _depth);

  std::cout << "Graph depth: " << graph_depth << endl;
  std::cout << "Depth of adding vertices: " << depth << endl;

  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_real_distribution<> dis(0, 1);

  for (int current_depth = 0; current_depth <= depth; current_depth++) {
    const double probability =
        static_cast<double>(current_depth) / static_cast<double>(depth);
    for (const auto& vertex : work_graph.get_vertices()) {
      if (vertex.depth == current_depth) {
        for (int iter = 0; iter < new_vertices_num; iter++) {
          if (dis(gen) > probability) {
            work_graph.add_vertex();
            work_graph.connect_vertices(
                vertex.id,
                work_graph.get_vertices()[work_graph.get_vertices_num() - 1].id,
                false);
          }
        }
      }
    }
  }
}

void paint_edges(Graph& work_graph) {
  const int graph_depth = work_graph.get_depth();

  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_real_distribution<> dis(0, 1);
  // BLUE

  for (int current_depth = 1; current_depth < graph_depth; current_depth++) {
    vector<Vertex> uni_depth_vertices;
    for (const auto& vertex : work_graph.get_vertices())
      if (vertex.depth == current_depth)
        uni_depth_vertices.emplace_back(vertex);

    std::array<VertexId, 2> adjacent_vertices = {INVALID_ID, INVALID_ID};
    for (const auto& vertex : uni_depth_vertices) {
      if (adjacent_vertices[0] == INVALID_ID) {
        adjacent_vertices[0] = vertex.id;
      } else if (adjacent_vertices[1] == INVALID_ID) {
        adjacent_vertices[1] = vertex.id;
        if (!work_graph.is_connected(adjacent_vertices[0],
                                     adjacent_vertices[1]))
          if (dis(gen) < 0.25)
            work_graph.connect_vertices(adjacent_vertices[0],
                                        adjacent_vertices[1], true);
      } else {
        adjacent_vertices[0] = adjacent_vertices[1];
        adjacent_vertices[1] = vertex.id;
        if (!work_graph.is_connected(adjacent_vertices[0],
                                     adjacent_vertices[1]))
          if (dis(gen) < 0.25)
            work_graph.connect_vertices(adjacent_vertices[0],
                                        adjacent_vertices[1], true);
      }
    }
  }

  for (const auto& start_vertex : work_graph.get_vertices()) {
    // GREEN
    if (!work_graph.is_looped(start_vertex.id)) {
      if (dis(gen) < 0.1) {
        work_graph.connect_vertices(start_vertex.id, start_vertex.id, true);
      }
    }
    // RED
    if (dis(gen) < 0.33) {
      if (start_vertex.depth + 2 <= graph_depth) {
        vector<VertexId> red_vertices_ids;
        for (const auto& end_vertex : work_graph.get_vertices()) {
          if (end_vertex.depth == start_vertex.depth + 2)
            if (!work_graph.is_connected(start_vertex.id, end_vertex.id))
              red_vertices_ids.emplace_back(end_vertex.id);
        }
        if (red_vertices_ids.size() > 0) {
          std::uniform_int_distribution<> distr(0, red_vertices_ids.size() - 1);
          work_graph.connect_vertices(start_vertex.id,
                                      red_vertices_ids[distr(gen)], true);
        }
      }
    }
    // YELLOW
    const double probability = static_cast<double>(start_vertex.depth) /
                               static_cast<double>(graph_depth);
    if (dis(gen) < probability) {
      vector<VertexId> yellow_vertices_ids;
      for (const auto& end_vertex : work_graph.get_vertices()) {
        if (end_vertex.depth == start_vertex.depth + 1) {
          if (!work_graph.is_connected(start_vertex.id, end_vertex.id))
            yellow_vertices_ids.push_back(end_vertex.id);
        }
      }
      if (yellow_vertices_ids.size() > 0) {
        std::uniform_int_distribution<> distr(0,
                                              yellow_vertices_ids.size() - 1);
        work_graph.connect_vertices(start_vertex.id,
                                    yellow_vertices_ids[distr(gen)], true);
      }
    }
  }
}

void write_log(const Graph& graph, std::ofstream& logfile) {
  logfile << graph.to_json();
}

int main() {
  std::ofstream log;
  log.open(LOG_FILENAME, std::ofstream::out | std::ofstream::app);

  int depth = INVALID_ID;
  do {
    std::cout << "Enter generate graph depth" << endl;
    std::cin >> depth;
  } while (depth < 0);
  int new_vertices_num = INVALID_NUMBER;
  do {
    std::cout << "Enter new_vertices_num" << endl;
    std::cin >> new_vertices_num;
  } while (new_vertices_num < 0);

  for (int graph_num = 0; graph_num < GRAPH_NUMBER; graph_num++) {
    Graph my_graph;
    for (int iter = 0; iter < START_VERTICES_NUMBER; iter++)
      my_graph.add_vertex();
    for (int vertex_number = 0; vertex_number < START_VERTICES_NUMBER - 1;
         vertex_number++)
      my_graph.connect_vertices(vertex_number, vertex_number + 1, false);

    new_vertices_generation(my_graph, depth, new_vertices_num);
    paint_edges(my_graph);
    write_graph(my_graph, graph_num);
    write_log(my_graph, log);
  }

  log.close();
  return 0;
}
