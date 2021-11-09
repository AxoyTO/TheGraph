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
EdgeColor Gray = EdgeColor::Gray;
EdgeColor Green = EdgeColor::Green;
EdgeColor Blue = EdgeColor::Blue;
EdgeColor Yellow = EdgeColor::Yellow;
EdgeColor Red = EdgeColor::Red;

std::random_device rd;
std::mt19937 mersenne(rd());

std::string color_to_string(const EdgeColor& color) {
  if (color == Gray) {
    return "\"gray\"";
  }
  if (color == Green) {
    return "\"green\"";
  }
  if (color == Blue) {
    return "\"blue\"";
  }
  if (color == Yellow) {
    return "\"yellow\"";
  } else {
    return "\"red\"";
  }
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
  Vertex(const VertexId& new_id, int depth_n) : id(new_id), depth(depth_n) {}
  const VertexId id = 0;
  int depth = 0;
  vector<EdgeId> children_ids;
  void add_edge_id(const EdgeId& id);
  const vector<EdgeId>& edge_ids() const { return edge_ids_; }
  //  const vector<EdgeId>& children_ids() const { return children_ids_; }
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
  const EdgeColor color = Gray;
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
  void add_vertex(int depth);

  void add_edge(const VertexId& begin,
                const VertexId& end,
                const EdgeColor& color);

  int depth() const { return depth_sizes_.size(); }
  int depth_size(int depth) const { return depth_sizes_[depth]; }
  int number_of_vertices() const { return vertices_.size(); }

  const vector<Vertex>& vertices() const { return vertices_; }
  const vector<Edge>& edges() const { return edges_; }

  bool has_vertex(const VertexId& vertex_id) const;
  bool is_connected(const VertexId& begin, const VertexId& end) const;
  void add_depth(int depth);

 private:
  VertexId num_of_vrt_ = 0;
  EdgeId num_of_edg_ = 0;

  VertexId next_vertex_id() { return num_of_vrt_++; }
  EdgeId next_edge_id() { return num_of_edg_++; }

  vector<Vertex> vertices_;
  vector<Edge> edges_;
  vector<int> depth_sizes_;
};

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
      if (begin != end || edges_[edge_num].color == Green)
        return true;
    }
  }
  return false;
}

void Graph::add_depth(int depth) {
  for (int i = depth_sizes_.size(); i <= depth; ++i)
    depth_sizes_.push_back(0);
}

void Graph::add_vertex(int depth) {
  vertices_.emplace_back(next_vertex_id(), depth);
  if (depth >= (int)depth_sizes_.size())
    add_depth(depth);
  ++depth_sizes_[depth];
}

void Graph::add_edge(const VertexId& begin,
                     const VertexId& end,
                     const EdgeColor& color) {
  assert(has_vertex(begin) && "Vertex doesn't exist");
  assert(has_vertex(end) && "Vertex doesn't exist");
  bool is_connect = is_connected(begin, end);
  bool color_gray = color == Gray;
  assert(!(is_connect && color_gray) && "Vertices already connected");
  if (is_connect)
    return;
  const auto& edge = edges_.emplace_back(next_edge_id(), begin, end, color);
  vertices_[begin].add_edge_id(edge.id);
  if (begin != end)
    vertices_[end].add_edge_id(edge.id);
  if (color_gray)
    vertices_[begin].children_ids.push_back(end);
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

int neighbour_choice(int rule, int k, int sub) {
  switch (rule) {
    case 1:  //есть только сосед на расстоянии sub с меньшим индексом
      return k - sub;
    case 2:  //есть только сосед на расстоянии sub с большим индексом
      return k + sub;
    case 3:  //есть оба соседа, подкидывем монетку, к кому кидать ребро
      const int coin = mersenne() % 2;
      return coin * (k - sub) + (1 - coin) * (k + sub);
  }
  return -1;
}
void generate_gray_edges(int graph_depth,
                         int new_vertices_num,
                         Graph& returned_graph,
                         vector<int>& offsets) {
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
    offsets.push_back(offset);
    offset_plus += returned_graph.depth_size(i);
    for (int k = offset; k < offset_plus; ++k) {
      for (int c = 0; c < new_vertices_num; ++c) {
        if ((int)(mersenne() % 100 + 1) <= p) {
          returned_graph.add_vertex(i + 1);
          returned_graph.add_edge(k, returned_graph.number_of_vertices() - 1,
                                  Gray);
        }
      }
    }
    offset = offset_plus;
    p -= sub_p;  //Уменьшили вероятность для следующего слоя
  }
  offsets.push_back(offset_plus);
  offsets.push_back(returned_graph.number_of_vertices());
}
void generate_blue_edges(Graph& returned_graph, vector<int>& offsets) {
  //Значения вероятостей возникновения ребер соответствующих цветов:
  const int p_blue = 25;

  for (int i = 0; i < returned_graph.depth(); ++i) {
    for (int k = offsets[i]; k < offsets[i + 1] - 1; ++k) {
      if (mersenne() % 100 + 1 <= p_blue) {
        returned_graph.add_edge(k, k + 1, Blue);
        ++k;
      }
    }
  }
}

bool is_children(VertexId& child_candidate,
                 VertexId& parent,
                 const Graph& graph) {
  for (const auto& edge_id : graph.vertices()[parent].children_ids) {
    if (child_candidate == edge_id) {
      return true;
    }
  }
  return false;
}

vector<VertexId> generate_set_vertices(VertexId& left_border,
                                       VertexId& right_border,
                                       VertexId& vert_id,
                                       const Graph& graph) {
  vector<VertexId> returned_vector;
  for (VertexId i = left_border; i < right_border; ++i) {
    if (is_children(i, vert_id, graph))
      continue;
    returned_vector.push_back(i);
  }
  return returned_vector;
}
void generate_green_edges(Graph& returned_graph, vector<int>& offsets) {
  //Значения вероятостей возникновения ребер соответствующих цветов:
  const int p_green = 10;
  for (int i = 0; i < returned_graph.depth(); ++i) {
    for (int k = offsets[i]; k < offsets[i + 1]; ++k) {
      if (mersenne() % 100 + 1 <= p_green) {
        returned_graph.add_edge(k, k, Green);
      }
    }
  }
}

void generate_yellow_edges(Graph& returned_graph, vector<int>& offsets) {
  int p_yellow = 0;
  //Настолько будет увеличиваться от слоя к слою вероятность возникновения
  //желтого ребра:
  int add_p_yellow = 100 / returned_graph.depth();

  for (int i = 0; i < returned_graph.depth() - 1; ++i) {
    for (int k = offsets[i]; k < offsets[i + 1]; ++k) {
      if ((int)(mersenne() % 100 + 1) <= p_yellow) {
        vector<VertexId> vertices_to_connect = generate_set_vertices(
            offsets[i + 1], offsets[i + 2], k, returned_graph);
        const int vertex_to_attach =
            vertices_to_connect[mersenne() % vertices_to_connect.size()];
        returned_graph.add_edge(k, vertex_to_attach, Yellow);
      }
    }
    //Увеличили вероятность появления желтых ребер
    //для следующего слоя
    p_yellow += add_p_yellow;
  }
}

void generate_red_edges(Graph& returned_graph, vector<int>& offsets) {
  const int p_red = 33;
  for (int i = 0; i < returned_graph.depth() - 2; ++i) {
    for (int k = offsets[i]; k < offsets[i + 1]; ++k) {
      if (i < returned_graph.depth() - 2) {
        if (mersenne() % 100 + 1 <= p_red) {
          const int vertex_to_attach =
              offsets[i + 2] + mersenne() % returned_graph.depth_size(i + 2);
          returned_graph.add_edge(k, vertex_to_attach, Red);
        }
      }
    }
  }
}

Graph generate_graph(int graph_depth,
                     int vrtx_on_first_lvl,
                     int new_vertices_num) {
  auto returned_graph = Graph();
  //Генерация вершин первого слоя:
  for (int i = 0; i < vrtx_on_first_lvl; ++i) {
    returned_graph.add_vertex(0);
  }
  //Вектор смещений слоев графа, используется для удобства
  vector<int> offsets;

  generate_gray_edges(graph_depth, new_vertices_num, returned_graph, offsets);
  generate_green_edges(returned_graph, offsets);
  generate_yellow_edges(returned_graph, offsets);
  generate_red_edges(returned_graph, offsets);
  generate_blue_edges(returned_graph, offsets);
  return returned_graph;
}

int main() {
  cout << "Enter graph depth: ";
  int depth;
  cin >> depth;
  cout << "Enter new vertices number: ";
  int new_vertices_num;
  cin >> new_vertices_num;
  try {
    if (depth < 0 || new_vertices_num < 0) {
      throw "You have entered negative values";
    }
    const Graph graph = generate_graph(depth, 1, new_vertices_num);
    std::ofstream out("graph_task_03.json");
    out << graph;
    out.close();
    return 0;
  } catch (const char* error) {
    cout << error << endl;
    return 1;
  }
}
