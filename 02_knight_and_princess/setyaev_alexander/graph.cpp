#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

class Vertex {
 public:
  Vertex(const int _id);
  const int get_id() const;
  std::vector<int> connected_edges;

 private:
  int id = 0;
};

struct Edge {
 public:
  Edge(const int i, const int first, const int second);
  const int get_id() const;
  const int get_first_id() const;
  const int get_second_id() const;

 private:
  int id = 0;
  int first_vertex = 0;
  int second_vertex = 0;
};

class Graph {
 public:
  void add_vertex(int i);
  void add_edge(int i, int first, int second);
  void vertex_input();
  void edge_input();
  std::string to_string() const;
  bool is_vertex_exists(int i) const;
  bool is_edge_exists(int i) const;

 private:
  std::vector<Vertex> vertices;
  std::vector<Edge> edges;
};

Vertex::Vertex(int i) {
  id = i;
}
Edge::Edge(int i, int first, int second) {
  id = i;
  first_vertex = first;
  second_vertex = second;
}

const int Vertex::get_id() const {
  return this->id;
}
const int Edge::get_id() const {
  return this->id;
}

const int Edge::get_first_id() const {
  return this->first_vertex;
}

const int Edge::get_second_id() const {
  return this->second_vertex;
}

void Graph::add_vertex(int i) {
  // Vertex tmp;
  // tmp.id = i;
  // vertices.push_back(tmp);

  vertices.emplace_back(i);
}

void json_to_file(std::string str);
void init();

void Graph::add_edge(int i, int first, int second) {
  for (int j = 0; j < vertices.size(); ++j) {
    if (vertices[j].get_id() == first) {
      vertices[j].connected_edges.push_back(i);
    }
    if (vertices[j].get_id() == second) {
      vertices[j].connected_edges.push_back(i);
    }
  }
  edges.emplace_back(i, first, second);
}

void init(Graph& g) {
  for (int i = 0; i < 14; ++i) {
    g.add_vertex(i);
  }
  g.add_edge(0, 0, 1);
  g.add_edge(1, 0, 2);
  g.add_edge(2, 0, 3);
  g.add_edge(3, 1, 4);
  g.add_edge(4, 1, 5);
  g.add_edge(5, 1, 6);
  g.add_edge(6, 2, 7);
  g.add_edge(7, 2, 8);
  g.add_edge(8, 3, 9);
  g.add_edge(9, 4, 10);
  g.add_edge(10, 5, 10);
  g.add_edge(11, 6, 10);
  g.add_edge(12, 7, 11);
  g.add_edge(13, 8, 11);
  g.add_edge(14, 9, 12);
  g.add_edge(15, 10, 13);
  g.add_edge(16, 11, 13);
  g.add_edge(17, 12, 13);
}

std::string Graph::to_string() const {
  std::stringstream json;

  json << "{" << std::endl << "	\"vertices\": [" << std::endl;

  for (int i = 0; i < vertices.size(); ++i) {
    json << "		{\n			\"id\": "
         << vertices[i].get_id() << "," << std::endl
         << "			\"edge_ids\": [";
    for (int j = 0; j < vertices[i].connected_edges.size(); ++j) {
      json << vertices[i].connected_edges[j];
      if (j < vertices[i].connected_edges.size() - 1) {
        json << ", ";
      } else {
        json << "]" << std::endl;
      }
    }
    json << "		}";
    if (i < vertices.size() - 1) {
      json << ",\n";
    } else {
      json << "\n	],\n	\"edges\": [\n";
    }
  }
  for (int i = 0; i < edges.size(); ++i) {
    json << "		{\n			\"id\": " << edges[i].get_id()
         << ",\n			\"vertex_ids\": ["
         << edges[i].get_first_id() << ", " << edges[i].get_second_id()
         << "]\n		}";
    if (i < edges.size() - 1) {
      json << ",\n";
    } else {
      json << "\n	]\n}";
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
  for (const auto& vertex : vertices) {
    if (vertex.get_id() == id) {
      return true;
    }
  }
  return false;
}

bool Graph::is_edge_exists(int id) const {
  for (const auto& edge : edges) {
    if (edge.get_id() == id) {
      return true;
    }
  }
  return false;
}

int main() {
  Graph graph;
  init(graph);
  std::string json_string = graph.to_string();
  json_to_file(json_string);

  return 0;
}
