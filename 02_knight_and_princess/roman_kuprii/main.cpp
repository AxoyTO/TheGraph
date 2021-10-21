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

constexpr int INVALID_ID = -1;
const std::string JSON_GRAPH_FILENAME = "graph.json";

int min(const int& a, const int& b) {
  return (a < b) ? a : b;
}

enum Colors { GRAY, GREEN, BLUE, YELLOW, RED };

struct Edge {
  const EdgeId id = INVALID_ID;
  const std::array<VertexId, 2> connected_vertices;
  Colors color = GRAY;

  Edge(const VertexId& start, const VertexId& end, const EdgeId& _id)
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
      case GRAY: {
        res += "\"gray\" }";
        break;
      }
      case GREEN: {
        res += "\"green\" }";
        break;
      }
      case BLUE: {
        res += "\"blue\" }";
        break;
      }
      case YELLOW: {
        res += "\"yellow\" }";
        break;
      }
      case RED: {
        res += "\"red\" }";
        break;
      }
      default:
        break;
    }
    return res;
  }

  void paint(const Colors& _color) { color = _color; }
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

  explicit Vertex(VertexId _id) : id(_id) {}

  std::string to_json() const {
    std::string res;
    res = "{ \"id\": ";
    res += to_string(id) + ", \"edge_ids\": [";
    for (int n : edges_ids_) {
      res += to_string(n);
      res += ", ";
    }
    res.pop_back();
    res.pop_back();
    res += "], \"depth\": ";
    res += to_string(depth);
    res += " }";
    return res;
  }

  void add_edge_id(const EdgeId& _id) {
    assert(!is_edge_id_included(_id, edges_ids_));
    edges_ids_.push_back(_id);
  }

  vector<EdgeId> get_edges_ids() const { return edges_ids_; }

 private:
  vector<EdgeId> edges_ids_;
};

bool is_vertex_id_included(const vector<Vertex>& vertices, const VertexId& id) {
  for (const auto& vert : vertices)
    if (vert.id == id)
      return true;
  return false;
}

class Graph {
 public:
  std::string to_json() const {
    std::string res;
    res = "{ \"depth\": ";
    res += to_string(depth_);
    res += ", \"vertices\": [ ";
    for (const auto& v_it : vertices_) {
      res += v_it.to_json();
      res += ", ";
    }
    res.pop_back();
    res.pop_back();
    res += " ], \"edges\": [ ";
    for (const auto& e_it : edges_) {
      res += e_it.to_json();
      res += ", ";
    }
    res.pop_back();
    res.pop_back();
    res += " ] }\n";
    return res;
  }

  void add_vertex() { vertices_.emplace_back(get_next_vertex_id()); }

  bool is_exist(const VertexId& vertex_id) const {
    for (const auto& vertex : vertices_) {
      if (vertex_id == vertex.id)
        return true;
    }
    return false;
  }

  bool is_connected(const VertexId& from_vertex_id,
                    const VertexId& to_vertex_id) const {
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
    vector<EdgeId> vertex_edges_ids = vertices_[vertex_id].get_edges_ids();
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
    // check if vertices exist
    assert(is_vertex_id_included(vertices_, from_vertex_id));
    assert(is_vertex_id_included(vertices_, to_vertex_id));

    // check if they are not connected
    if (from_vertex_id != to_vertex_id)
      assert(!is_connected(from_vertex_id, to_vertex_id));
    else
      assert(!is_looped(from_vertex_id));

    const auto& new_edge =
        edges_.emplace_back(from_vertex_id, to_vertex_id, get_next_edge_id());

    // add information into Verex structure
    vertices_[from_vertex_id].add_edge_id(new_edge.id);
    if (from_vertex_id != to_vertex_id)
      vertices_[to_vertex_id].add_edge_id(new_edge.id);

    if (!paint) {
      int min_depth = vertices_[from_vertex_id].depth;
      for (const auto& edge_idx : vertices_[to_vertex_id].get_edges_ids()) {
        VertexId vert = edges_[edge_idx].connected_vertices[0];
        min_depth = min(min_depth, vertices_[vert].depth);
      }
      vertices_[to_vertex_id].depth = (min_depth + 1);

      if (depth_ < (min_depth + 1))
        depth_ = min_depth + 1;
    }

    if (paint) {
      int diff =
          vertices_[to_vertex_id].depth - vertices_[from_vertex_id].depth;
      if (from_vertex_id == to_vertex_id) {
        edges_[new_edge.id].color = GREEN;
      } else if (diff == 0) {
        edges_[new_edge.id].color = BLUE;
      } else if (diff == 1) {
        edges_[new_edge.id].color = YELLOW;
      } else if (diff == 2) {
        edges_[new_edge.id].color = RED;
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

void write_graph(const Graph& graph) {
  std::ofstream out;
  out.open(JSON_GRAPH_FILENAME, std::ofstream::out | std::ofstream::trunc);

  out << graph.to_json();

  out.close();
}

void new_vertices_generation(Graph* work_graph) {
  int depth;
  std::cout << "Enter generate graph depth" << endl;
  std::cin >> depth;
  assert(depth >= 0);
  int new_vertices_num;
  std::cout << "Enter new_vertices_num" << endl;
  std::cin >> new_vertices_num;

  int graph_depth = work_graph->get_depth();
  depth = min(graph_depth, depth);

  std::cout << "Graph depth: " << graph_depth << endl;
  std::cout << "min depth: " << depth << endl;

  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_real_distribution<> dis(0, 1);

  for (int i = 0; i <= depth; i++) {
    double probability = static_cast<double>(i) / static_cast<double>(depth);
    for (const auto& vertex : work_graph->get_vertices()) {
      if (vertex.depth == i) {
        for (int j = 0; j < new_vertices_num; j++) {
          if (dis(gen) > probability) {
            work_graph->add_vertex();
            work_graph->connect_vertices(
                vertex.id, work_graph->get_vertices_num() - 1, false);
          }
        }
      }
    }
  }
}

void paint_edges(Graph* work_graph) {
  int graph_depth = work_graph->get_depth();

  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_real_distribution<> dis(0, 1);
  // BLUE
  std::array<VertexId, 2> adjacent_vertices = {INVALID_ID, INVALID_ID};
  for (const auto& vertex : work_graph->get_vertices()) {
    adjacent_vertices[0] = INVALID_ID;
    adjacent_vertices[1] = INVALID_ID;
    for (const auto& edge_id : vertex.get_edges_ids()) {
      Edge edge = work_graph->get_edges()[edge_id];
      if (edge.connected_vertices[0] == vertex.id) {
        if (adjacent_vertices[0] == INVALID_ID) {
          adjacent_vertices[0] = edge.connected_vertices[1];
        } else if (adjacent_vertices[1] == INVALID_ID) {
          adjacent_vertices[1] = edge.connected_vertices[1];
          if (dis(gen) < 0.25)
            work_graph->connect_vertices(adjacent_vertices[0],
                                         adjacent_vertices[1], true);
        } else {
          adjacent_vertices[0] = adjacent_vertices[1];
          adjacent_vertices[1] = edge.connected_vertices[1];
          if (dis(gen) < 0.25)
            work_graph->connect_vertices(adjacent_vertices[0],
                                         adjacent_vertices[1], true);
        }
      }
    }
  }

  for (const auto& vertex : work_graph->get_vertices()) {
    // GREEN
    if (dis(gen) < 0.1) {
      work_graph->connect_vertices(vertex.id, vertex.id, true);
    }
    // RED
    if (dis(gen) < 0.33) {
      if ((vertex.depth + 2) <= graph_depth) {
        vector<VertexId> red_vertices_ids;
        for (const auto& tmp_vertex : work_graph->get_vertices()) {
          if (tmp_vertex.depth == (vertex.depth + 2))
            red_vertices_ids.emplace_back(tmp_vertex.id);
        }
        if (red_vertices_ids.size() > 0) {
          std::uniform_int_distribution<> distr(0, red_vertices_ids.size() - 1);
          work_graph->connect_vertices(vertex.id, red_vertices_ids[distr(gen)],
                                       true);
        }
      }
    }
    // YELLOW
    double probability =
        static_cast<double>(vertex.depth) / static_cast<double>(graph_depth);
    if (dis(gen) < probability) {
      vector<VertexId> yellow_vertices_ids;
      for (const auto& tmp_vertex : work_graph->get_vertices()) {
        if (tmp_vertex.depth == (vertex.depth + 1)) {
          vector<Edge> edges = work_graph->get_edges();
          bool check = true;
          for (const auto& edge_ids : vertex.get_edges_ids())
            if (edges[edge_ids].connected_vertices[1] == tmp_vertex.id) {
              check = false;
              break;
            }
          if (check)
            yellow_vertices_ids.push_back(tmp_vertex.id);
        }
      }
      if (yellow_vertices_ids.size() > 0) {
        std::uniform_int_distribution<> distr(0,
                                              yellow_vertices_ids.size() - 1);
        work_graph->connect_vertices(vertex.id, yellow_vertices_ids[distr(gen)],
                                     true);
      }
    }
  }
}

int main() {
  Graph my_graph;
  for (int i = 0; i < 14; i++) {
    my_graph.add_vertex();
  }
  my_graph.connect_vertices(0, 1, false);
  my_graph.connect_vertices(0, 2, false);

  my_graph.connect_vertices(0, 3, false);
  my_graph.connect_vertices(1, 4, false);
  my_graph.connect_vertices(1, 5, false);
  my_graph.connect_vertices(1, 6, false);
  my_graph.connect_vertices(2, 7, false);
  my_graph.connect_vertices(2, 8, false);
  my_graph.connect_vertices(3, 9, false);
  my_graph.connect_vertices(4, 10, false);
  my_graph.connect_vertices(5, 10, false);
  my_graph.connect_vertices(6, 10, false);
  my_graph.connect_vertices(7, 11, false);
  my_graph.connect_vertices(8, 11, false);
  my_graph.connect_vertices(9, 12, false);
  my_graph.connect_vertices(10, 13, false);
  my_graph.connect_vertices(11, 13, false);
  my_graph.connect_vertices(12, 13, false);

  new_vertices_generation(&my_graph);

  paint_edges(&my_graph);

  write_graph(my_graph);

  return 0;
}
