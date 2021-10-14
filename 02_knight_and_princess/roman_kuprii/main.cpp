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

int MIN(const int& a, const int& b) {
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

  void add_edge_id(const EdgeId& _id) { edges_ids_.push_back(_id); }

  std::vector<EdgeId> get_edges_ids() const { return edges_ids_; }

 private:
  std::vector<EdgeId> edges_ids_;
};

bool vertex_check(const std::vector<Vertex>& vertices, VertexId id) {
  bool check = 0;
  for (const auto& vert : vertices)
    if (vert.id == id)
      check = 1;
  return check;
}

bool edge_connection_check(const std::vector<Edge>& edges,
                           VertexId out_id,
                           VertexId dest_id) {
  bool check = 0;
  for (const auto& edge : edges) {
    if (edge.connected_vertices[0] == out_id &&
        edge.connected_vertices[1] == dest_id)
      check = 1;
  }
  return check;
}

class Graph {
 public:
  Graph() {}

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

  void add_vertex() { vertices_.emplace_back(vertices_.size()); }

  void connect_vertices(const VertexId& out_id,
                        const VertexId& dest_id,
                        bool paint) {
    // check if vertices exist
    assert(vertex_check(vertices_, out_id) == 1);
    assert(vertex_check(vertices_, dest_id) == 1);

    // check if they are not connected
    assert(edge_connection_check(edges_, out_id, dest_id) == 0);

    EdgeId id = edges_.size();
    Edge new_edge(out_id, dest_id, id);
    edges_.push_back(new_edge);

    // add information into Verex structure
    vertices_[out_id].add_edge_id(id);
    vertices_[dest_id].add_edge_id(id);

    if (!paint) {
      int min_depth = vertices_[out_id].depth;
      for (const auto& edge_idx : vertices_[dest_id].get_edges_ids()) {
        VertexId vert = edges_[edge_idx].connected_vertices[0];
        min_depth = MIN(min_depth, vertices_[vert].depth);
      }
      vertices_[dest_id].depth = (min_depth + 1);

      if (depth_ < (min_depth + 1))
        depth_ = min_depth + 1;
    }

    if (paint) {
      int diff = vertices_[dest_id].depth - vertices_[out_id].depth;
      if (out_id == dest_id) {
        edges_[id].color = GREEN;
      } else if (diff == 0) {
        edges_[id].color = BLUE;
      } else if (diff == 1) {
        edges_[id].color = YELLOW;
      } else if (diff == 2) {
        edges_[id].color = RED;
      }
    }
  }

  vector<Edge> get_edges() const { return edges_; }
  vector<Vertex> get_vertices() const { return vertices_; }

  int get_graph_depth() const { return depth_; }
  int get_vertices_num() const { return vertices_.size(); }

 private:
  vector<Vertex> vertices_;
  vector<Edge> edges_;
  int depth_ = 0;
};

void write_graph(const Graph& graph) {
  std::ofstream out;
  out.open(JSON_GRAPH_FILENAME, std::ofstream::out | std::ofstream::trunc);

  out << graph.to_json();

  out.close();
}

void new_vertex_generation(Graph* work_graph) {
  int depth;
  std::cout << "Enter generate graph depth" << endl;
  std::cin >> depth;
  assert(depth >= 0);
  int new_vertices_num;
  std::cout << "Enter new_vertices_num" << endl;
  std::cin >> new_vertices_num;

  int graph_depth = work_graph->get_graph_depth();
  depth = MIN(graph_depth, depth);

  std::cout << "Graph depth: " << graph_depth << endl;
  std::cout << "min depth: " << depth << endl;

  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_real_distribution<> dis(0, 1);

  for (int i = 0; i <= depth; i++) {
    double probability = (double)i / (double)depth;
    std::cout << probability << endl;
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

  new_vertex_generation(&my_graph);

  int graph_depth = my_graph.get_graph_depth();
  std::cout << "Graph depth: " << graph_depth << std::endl;

  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_real_distribution<> dis(0, 1);

/*
//BLUE
    std::array<VertexId, 2> adjacent_vertices = {INVALID_ID, INVALID_ID};
    for (const auto& vertex : my_graph.get_vertices()) {

        for (const auto& edge : vertex.get_edges_ids()) {

            if (edge.connected_vertices[0] = vertex.id) {
                if (adjacent_vertices[0] == INVALID_ID) {
                    adjacent_vertices[0] = edge.connected_vertices[1];
                } else if (adjacent_vertices[1] == INVALID_ID) {
                    adjacent_vertices[1] = edge.connected_vertices[1];
                    my_graph.connect_vertices(adjacent_vertices[0], adjacent_vertices[1]);
                } else {
                    adjacent_vertices

                ????


*/



  for (const auto& vertex : my_graph.get_vertices()) {
    if (dis(gen) < 0.1) {
      my_graph.connect_vertices(vertex.id, vertex.id, true);
    }
    if (dis(gen) < 0.25) {
      // connect two adjacent vertices
      // todo: find adjacent vertex
      VertexId adjacent_vertex = INVALID_ID;
      for (const auto& tmp_vertex : my_graph.get_vertices()) {
        if (tmp_vertex.depth == vertex.depth) {
          if (edge_connection_check(my_graph.get_edges(), vertex.id,
                                    tmp_vertex.id) == 0) {
            adjacent_vertex = tmp_vertex.id;
            break;
          }
        }
      }
      my_graph.connect_vertices(vertex.id, adjacent_vertex, true);
    }
    if (dis(gen) < 0.33) {
      if ((vertex.depth + 2) <= graph_depth) {
        std::vector<VertexId> red_vertices_ids;
        for (const auto& tmp_vertex : my_graph.get_vertices()) {
          if (tmp_vertex.depth == (vertex.depth + 2))
            red_vertices_ids.emplace_back(tmp_vertex.id);
        }
        if (red_vertices_ids.size() > 0) {
          std::random_device rd;
          std::mt19937 eng(rd());
          std::uniform_int_distribution<> distr(0, red_vertices_ids.size() - 1);
          my_graph.connect_vertices(vertex.id, red_vertices_ids[distr(eng)],
                                    true);
        }
      }
    }

    if (false) {
      // randomly connect with random vertex on the next layer (YELLOW)
    }
  }

  write_graph(my_graph);

  return 0;
}
