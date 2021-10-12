#include <random>
#include <cassert>
#include <array>
#include <fstream>
#include <iostream>
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

enum Colors {GRAY, GREEN, BLUE, YELLOW, RED};

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
    switch (color)
    {
        case GRAY:
        {
            res += "\"gray\" }";
            break;
        }
        case GREEN:
        {
            res += "\"green\" }";
            break;
        }
        case BLUE:
        {
            res += "\"blue\" }";
            break;
        }
        case YELLOW:
        {
//FIXME
            res += "\"gray\" }";
            break;
        }
        case RED:
        {
            res += "\"red\" }";
            break;
        }
        default:
            break;
    }

    return res;
  }
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
    for (int n : edges_ids) {
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

  void add_edge_id(const EdgeId& _id) { edges_ids.push_back(_id); }

    std::vector<EdgeId> get_edges_ids() const {
        return edges_ids;
    }

private:
  std::vector<EdgeId> edges_ids;

};

class Graph {
 public:
    Graph() {}

  std::string to_json() const {
    std::string res;
    res = "{ \"depth\": ";
    res += to_string(depth);
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

  void add_vertex() {
//it is okey, until function remove vertex is added
    Vertex new_vertex(vertices_.size());
    vertices_.push_back(new_vertex);
  }

  void connect_vertices(VertexId out_id, VertexId dest_id) {
//it is okey, until function remove edge is added
    EdgeId id = edges_.size();
    Edge new_edge(out_id, dest_id, id);
    edges_.push_back(new_edge);

    // add information into Verex structure
    vertices_[out_id].add_edge_id(id);
    vertices_[dest_id].add_edge_id(id);

    int min_depth = vertices_[out_id].depth;
    for (const auto& edge_idx : vertices_[dest_id].get_edges_ids()) {
        VertexId vert = edges_[edge_idx].connected_vertices[0];
        min_depth = MIN(min_depth, vertices_[vert].depth);
    }
    vertices_[dest_id].depth = min_depth + 1;

// graph depth
    if (depth < min_depth+1) depth = min_depth+1;

//TODO make edge method - "add color"
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

    vector<Vertex> get_vertices () const {return vertices_; }

  int get_graph_depth() const { return depth; }
  int get_vertices_num() const { return vertices_.size(); }

 private:
  vector<Vertex> vertices_;
  vector<Edge> edges_;
  int depth = 0;
};

void write_graph(const Graph& graph) {
  std::ofstream out;
  out.open(JSON_GRAPH_FILENAME, std::ofstream::out | std::ofstream::trunc);

  out << graph.to_json();

  out.close();
}

int main() {
    Graph my_graph;
    for (int i = 0; i < 14; i++) {
        my_graph.add_vertex();
    }
    my_graph.connect_vertices(0, 1);
    my_graph.connect_vertices(0, 2);

    my_graph.connect_vertices(0, 3);
    my_graph.connect_vertices(1, 4);
    my_graph.connect_vertices(1, 5);
    my_graph.connect_vertices(1, 6);
    my_graph.connect_vertices(2, 7);
    my_graph.connect_vertices(2, 8);
    my_graph.connect_vertices(3, 9);
    my_graph.connect_vertices(4, 10);
    my_graph.connect_vertices(5, 10);
    my_graph.connect_vertices(6, 10);
    my_graph.connect_vertices(7, 11);
    my_graph.connect_vertices(8, 11);
    my_graph.connect_vertices(9, 12);
    my_graph.connect_vertices(10, 13);
    my_graph.connect_vertices(11, 13);
    my_graph.connect_vertices(12, 13);

//TEST
  for (int i = 0; i < 4; i++) {
    my_graph.add_vertex();
    my_graph.connect_vertices(0, my_graph.get_vertices_num() - 1);
  }

    int depth;
    std::cout << "Enter graph depth" << endl;
    std::cin >> depth;
    assert(depth >= 0);
    int new_vertices_num;
    std::cout << "Enter new_vertices_num" << endl;
    std::cin >> new_vertices_num;

    depth = MIN(my_graph.get_graph_depth(), depth);

    std::cout << "Graph depth: " << my_graph.get_graph_depth() << endl;
    std::cout << "min depth: " << depth << endl;

std::random_device rd;
std::mt19937 gen(rd());
std::uniform_real_distribution<> dis(0, 1);

    for (int i = 0; i <= depth; i++) {
        double probability = (double)i / (double)my_graph.get_graph_depth();
        std::cout << probability << endl;
        if (dis(gen) > probability) {

//      FOR EVERY VERTEX ON THIS LVL
            for (const auto& vertex : my_graph.get_vertices()) {
                if (vertex.depth == i) {
                    for (int j = 0; j < new_vertices_num; j++) {
                        my_graph.add_vertex();
                        my_graph.connect_vertices(vertex.id, my_graph.get_vertices_num()-1);
                    }
                }
            }
        }
    }

  write_graph(my_graph);

  return 0;
}
