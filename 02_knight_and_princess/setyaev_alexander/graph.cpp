#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

class Vertex {
 public:
  Vertex(int id);
  int get_id() const;
  std::vector<int> connected_edges;

 private:
  int _id = 0;
};

struct Edge {
 public:
  Edge(int id, int first_vertex_id, int second_vertex_id);
  int get_id() const;
  int get_first_id() const;
  int get_second_id() const;

 private:
  int _id = 0;
  int _first_vertex_id = 0;
  int _second_vertex_id = 0;
};

class Graph {
 public:
  void add_vertex();
  void add_edge(int first_vertex_id, int second_vertex_id);
  std::string to_string() const;
  bool is_vertex_exists(int id) const;
  bool is_edge_exists(int id) const;
  int get_new_vertex_id() const;
  int get_new_edge_id() const;

 private:
  std::vector<Vertex> _vertices;
  std::vector<Edge> _edges;
};

int Vertex::get_id() const {
  return _id;
}
int Edge::get_id() const {
  return _id;
}

int Edge::get_first_id() const {
  return _first_vertex_id;
}

int Edge::get_second_id() const {
  return _second_vertex_id;
}

Edge::Edge(int id, int first_vertex_id, int second_vertex_id) {
  _id = id;
  _first_vertex_id = first_vertex_id;
  _second_vertex_id = second_vertex_id;
}

Vertex::Vertex(int id) {
  _id = id;
}

void Graph::add_vertex() {
  _vertices.emplace_back(get_new_vertex_id());
}

int Graph::get_new_vertex_id() const {
  return _vertices.size();
}
int Graph::get_new_edge_id() const {
  return _edges.size();
}

void Graph::add_edge(int first, int second) {
  for (int j = 0; j < _vertices.size(); ++j) {
    if (_vertices[j].get_id() == first) {
      _vertices[j].connected_edges.push_back(get_new_edge_id());
    }
    if (_vertices[j].get_id() == second) {
      _vertices[j].connected_edges.push_back(get_new_edge_id());
    }
  }
  _edges.emplace_back(get_new_edge_id(), first, second);
}

Graph generate_graph() {
  Graph g;
  const int VERTICES_COUNT = 14;

  for (int i = 0; i < VERTICES_COUNT; ++i) {
    g.add_vertex();
  }

  g.add_edge(0, 1);
  g.add_edge(0, 2);
  g.add_edge(0, 3);
  g.add_edge(1, 4);
  g.add_edge(1, 5);
  g.add_edge(1, 6);
  g.add_edge(2, 7);
  g.add_edge(2, 8);
  g.add_edge(3, 9);
  g.add_edge(4, 10);
  g.add_edge(5, 10);
  g.add_edge(6, 10);
  g.add_edge(7, 11);
  g.add_edge(8, 11);
  g.add_edge(9, 12);
  g.add_edge(10, 13);
  g.add_edge(11, 13);
  g.add_edge(12, 13);

  return g;
}

std::string Graph::to_string() const {
  std::stringstream json;

  json << "{" << std::endl << "	\"vertices\": [" << std::endl;

  for (int i = 0; i < _vertices.size(); ++i) {
    json << "		{\n			\"id\": "
         << _vertices[i].get_id() << "," << std::endl
         << "			\"edge_ids\": [";
    for (int j = 0; j < _vertices[i].connected_edges.size(); ++j) {
      json << _vertices[i].connected_edges[j];
      if (j < _vertices[i].connected_edges.size() - 1) {
        json << ", ";
      } else {
        json << "]" << std::endl;
      }
    }
    json << "		}";
    if (i < _vertices.size() - 1) {
      json << ",\n";
    } else {
      json << "\n	],\n	\"edges\": [\n";
    }
  }
  for (int i = 0; i < _edges.size(); ++i) {
    json << "		{\n			\"id\": " << _edges[i].get_id()
         << ",\n			\"vertex_ids\": ["
         << _edges[i].get_first_id() << ", " << _edges[i].get_second_id()
         << "]\n		}";
    if (i < _edges.size() - 1) {
      json << ",\n";
    } else {
      json << "\n	]\n}\n";
    }
  }

  return json.str();
}

void json_to_file(std::string str) {
  std::ofstream json;
  json.open("graph.json");
  if (!json.is_open()) {
    throw std::runtime_error("Can't open graph.json file");
  }
  json << str;
  json.close();
}

bool Graph::is_vertex_exists(int id) const {
  for (const auto& vertex : _vertices) {
    if (vertex.get_id() == id) {
      return true;
    }
  }
  return false;
}

bool Graph::is_edge_exists(int id) const {
  for (const auto& edge : _edges) {
    if (edge.get_id() == id) {
      return true;
    }
  }
  return false;
}

int main() {
  const Graph graph = generate_graph();
  const std::string json_string = graph.to_string();
  json_to_file(json_string);

  return 0;
}
