#include <cassert>
#include <fstream>
#include <iostream>
#include <random>
#include <vector>

using std::cin;
using std::cout;
using std::endl;
using std::vector;

using VertexId = int;
using EdgeId = int;

enum class EdgeColor { Gray, Green, Blue, Yellow, Red };

std::string color_to_string(const EdgeColor& color) {
  switch (color) {
    case EdgeColor::Gray:
      return "\"gray\"";
    case EdgeColor::Green:
      return "\"green\"";
    case EdgeColor::Blue:
      return "\"blue\"";
    case EdgeColor::Yellow:
      return "\"yellow\"";
    case EdgeColor::Red:
      return "\"red\"";
  }
  throw "Color output error";
}

std::ostream& operator<<(std::ostream& out, const vector<int>& int_vector) {
  for (auto it = int_vector.begin(); it != int_vector.end(); ++it) {
    out << *it;
    if (it != int_vector.end() - 1) {
      out << ", ";
    }
  }
  return out;
}

struct Vertex {
  explicit Vertex(const VertexId& new_id) : id(new_id) {}
  const VertexId id = 0;
  int depth = 0;
  void add_edge_id(const EdgeId& id);
  const vector<EdgeId>& edge_ids() const { return edge_ids_; }
  bool has_edge_id(const EdgeId& id) const;

 private:
  vector<EdgeId> edge_ids_;
};

bool Vertex::has_edge_id(const EdgeId& id) const {
  for (const auto& edge_id : edge_ids_)
    if (id == edge_id)
      return true;
  return false;
}

void Vertex::add_edge_id(const EdgeId& id) {
  assert(!has_edge_id(id) && "Edge doesn't exist");
  edge_ids_.push_back(id);
}

std::ostream& operator<<(std::ostream& out, const Vertex& vertex) {
  out << "{" << endl;
  out << "      \"id\": " << vertex.id << "," << endl;
  out << "      \"edge_ids\": [" << vertex.edge_ids() << "]," << endl;
  out << "      \"depth\": " << vertex.depth << endl;
  out << "}";
  return out;
}

struct Edge {
  Edge(const EdgeId& new_id,
       const VertexId& begin_vertex,
       const VertexId& end_vertex,
       const EdgeColor color)
      : id(new_id), begin(begin_vertex), end(end_vertex), color(color) {}
  const EdgeId id = 0;
  const VertexId begin = 0;
  const VertexId end = 0;
  const EdgeColor color = EdgeColor::Gray;
};

std::ostream& operator<<(std::ostream& out, const Edge& edge) {
  out << "{" << endl;
  out << "      \"id\": " << edge.id << "," << endl;
  out << "      \"vertex_ids\": [" << edge.begin << ", " << edge.end << "],"
      << endl;
  out << "      \"color\": ";
  out << color_to_string(edge.color);
  out << endl;
  out << "}";
  return out;
}

class Graph {
 public:
  void add_vertex();

  void add_edge(const VertexId& begin,
                const VertexId& end,
                const EdgeColor& color);

  int depth() const { return depth_map_.size(); }
  int number_of_vertices() const { return vertices_.size(); }

  const vector<Vertex>& vertices() const { return vertices_; }
  const vector<Edge>& edges() const { return edges_; }
  const vector<vector<VertexId>>& depth_map() const { return depth_map_; }
  bool has_vertex(const VertexId& vertex_id) const;
  bool is_connected(const VertexId& begin, const VertexId& end) const;
  void mod_depth(const VertexId& begin, const VertexId& end);

 private:
  VertexId num_of_vrt_ = 0;
  EdgeId num_of_edg_ = 0;

  VertexId next_vertex_id() { return num_of_vrt_++; }
  EdgeId next_edge_id() { return num_of_edg_++; }

  vector<Vertex> vertices_;
  vector<Edge> edges_;
  vector<vector<VertexId>> depth_map_;
};
void Graph::mod_depth(const VertexId& begin, const VertexId& end) {
  int depth_for_new_vertex = 0;
  if (begin != end)
    depth_for_new_vertex = vertices_[begin].depth + 1;
  if (depth_for_new_vertex >= (int)depth_map_.size()) {
    vector<VertexId> tmp_vec = {end};
    depth_map_.push_back(tmp_vec);
  } else {
    depth_map_[depth_for_new_vertex].push_back(end);
  }
  vertices_[end].depth = depth_for_new_vertex;
}

bool Graph::has_vertex(const VertexId& vertex_id) const {
  for (const auto& vertex : vertices_) {
    if (vertex_id == vertex.id)
      return true;
  }
  return false;
}

bool Graph::is_connected(const VertexId& begin, const VertexId& end) const {
  assert(has_vertex(begin) && "Vertex doesn't exist");
  assert(has_vertex(end) && "Vertex doesn't exist");
  for (const EdgeId& edge_num : vertices_[begin].edge_ids()) {
    if (edges_[edge_num].begin == end || edges_[edge_num].end == end) {
      if (begin != end || edges_[edge_num].color == EdgeColor::Green)
        return true;
    }
  }
  return false;
}

void Graph::add_vertex() {
  vertices_.emplace_back(next_vertex_id());
}

void Graph::add_edge(const VertexId& begin,
                     const VertexId& end,
                     const EdgeColor& color) {
  assert(has_vertex(begin) && "Vertex doesn't exist");
  assert(has_vertex(end) && "Vertex doesn't exist");
  assert(!(is_connected(begin, end)) && "Vertices already connected");
  const auto& edge = edges_.emplace_back(next_edge_id(), begin, end, color);
  vertices_[begin].add_edge_id(edge.id);
  if (begin != end)
    vertices_[end].add_edge_id(edge.id);
  if (color == EdgeColor::Gray)
    mod_depth(begin, end);
}

std::ostream& operator<<(std::ostream& out, const vector<Vertex>& layer) {
  for (auto it = layer.begin(); it != layer.end(); ++it) {
    out << *it;
    if (it != layer.end() - 1) {
      out << ", ";
    }
  }
  return out;
}

std::ostream& operator<<(std::ostream& out, const vector<Edge>& edges) {
  for (auto it = edges.begin(); it != edges.end(); ++it) {
    out << *it;
    if (it != edges.end() - 1) {
      out << ", ";
    }
  }
  return out;
}

std::ostream& operator<<(std::ostream& out, const Graph& graph) {
  out << "{" << endl;
  out << "  \"depth\": " << graph.depth() << "," << endl;
  out << "  \"vertices\": [" << endl
      << "   " << graph.vertices() << endl
      << "  ]," << endl;
  out << "  \"edges\": [" << endl
      << "    " << graph.edges() << endl
      << "  ]" << endl
      << "}" << endl;
  return out;
}

void generate_gray_edges(int graph_depth,  //наибольшая возможная глубина
                         int new_vertices_num,
                         Graph& returned_graph) {
  //Генерация нулевой вершины:
  returned_graph.add_vertex();
  returned_graph.mod_depth(0, 0);
  // returned_graph.increase_the_depth(0);

  std::random_device rd;
  std::mt19937 mersenne(rd());
  //Вероятность порождения вершины следующего слоя:
  int p = 100;
  //Настолько вероятность будет уменьшена для следующего слоя:
  int sub_p = p / graph_depth;
  //Генерация вершин начиная со второго слоя:
  int offset = 0;
  int offset_plus = 0;

  for (int i = 0; i < graph_depth - 1; ++i) {
    if (i >= returned_graph.depth()) {
      break;
    }
    offset_plus += returned_graph.depth_map()[i].size();
    for (int k = offset; k < offset_plus; ++k) {
      for (int c = 0; c < new_vertices_num; ++c) {
        if ((int)(mersenne() % 100 + 1) <= p) {
          returned_graph.add_vertex();
          returned_graph.add_edge(k, returned_graph.number_of_vertices() - 1,
                                  EdgeColor::Gray);
        }
      }
    }
    offset = offset_plus;
    p -= sub_p;  //Уменьшили вероятность для следующего слоя
  }
}
void generate_blue_edges(Graph& returned_graph) {
  std::random_device rd;
  std::mt19937 mersenne(rd());
  //Значения вероятостей возникновения ребер соответствующих цветов:
  const int p_blue = 25;
  for (auto& vector_ids : returned_graph.depth_map()) {
    for (auto k = vector_ids.begin(); k < vector_ids.end() - 1; ++k) {
      if (mersenne() % 100 + 1 <= p_blue) {
        returned_graph.add_edge(*k, *(k + 1), EdgeColor::Blue);
        ++k;
      }
    }
  }
}

vector<VertexId> generate_set_vertices(const vector<VertexId>& layer,
                                       const VertexId& vert_id,
                                       const Graph& graph) {
  vector<VertexId> returned_vector;
  for (auto& vertex_id : layer) {
    if (graph.is_connected(vert_id, vertex_id))
      continue;
    returned_vector.push_back(vertex_id);
  }
  return returned_vector;
}
void generate_green_edges(Graph& returned_graph) {
  std::random_device rd;
  std::mt19937 mersenne(rd());
  const int p_green = 10;

  for (auto& vertex : returned_graph.vertices()) {
    if (mersenne() % 100 + 1 <= p_green) {
      returned_graph.add_edge(vertex.id, vertex.id, EdgeColor::Green);
    }
  }
}

void generate_yellow_edges(Graph& returned_graph) {
  std::random_device rd;
  std::mt19937 mersenne(rd());
  int p_yellow = 0;
  //Настолько будет увеличиваться от слоя к слою вероятность возникновения
  //желтого ребра:
  int add_p_yellow = 100 / returned_graph.depth();

  for (auto i = returned_graph.depth_map().begin();
       i != returned_graph.depth_map().end() - 1; ++i) {
    for (auto k = (*i).begin(); k != (*i).end(); ++k) {
      if ((int)(mersenne() % 100 + 1) <= p_yellow) {
        vector<VertexId> vertices_to_connect =
            generate_set_vertices(*(i + 1), *k, returned_graph);
        const int vertex_to_attach =
            vertices_to_connect[mersenne() % vertices_to_connect.size()];
        returned_graph.add_edge(*k, vertex_to_attach, EdgeColor::Yellow);
      }
    }
    p_yellow += add_p_yellow;
  }
}

void generate_red_edges(Graph& returned_graph) {
  std::random_device rd;
  std::mt19937 mersenne(rd());
  const int p_red = 33;

  for (auto i = returned_graph.depth_map().begin();
       i != returned_graph.depth_map().end() - 2; ++i) {
    for (auto k = (*i).begin(); k != (*i).end(); ++k) {
      if ((int)(mersenne() % 100 + 1) <= p_red) {
        const int vertex_to_attach = (*(i + 2))[mersenne() % (*(i + 2)).size()];
        returned_graph.add_edge(*k, vertex_to_attach, EdgeColor::Red);
      }
    }
  }
}

Graph generate_graph(int graph_depth, int new_vertices_num) {
  auto returned_graph = Graph();

  generate_gray_edges(graph_depth, new_vertices_num, returned_graph);
  generate_green_edges(returned_graph);
  generate_yellow_edges(returned_graph);
  generate_red_edges(returned_graph);
  generate_blue_edges(returned_graph);
  return returned_graph;
}

int main() {
  int depth;
  do {
    cout << "Enter graph depth: ";
    cin >> depth;
  } while (depth < 0);

  int new_vertices_num;
  do {
    cout << "Enter new vertices number: ";
    cin >> new_vertices_num;
  } while (depth < 0);

  try {
    const Graph graph = generate_graph(depth, new_vertices_num);
    std::ofstream out("graph_task_03.json");
    out << graph;
    out.close();
    return 0;
  } catch (const char* error) {
    cout << error << endl;
    return 1;
  }
}
