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

constexpr int PROBA_GRAY_BEGIN = 100;
constexpr int PROBA_RED = 33;
constexpr int PROBA_GREEN = 10;
constexpr int PROBA_BLUE = 25;

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
  throw std::runtime_error("Color output error");
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
  VertexId add_vertex();

  void add_edge(const VertexId& begin,
                const VertexId& end,
                const EdgeColor& color);

  int depth() const { return depth_map_.size() - 1; }

  const vector<Vertex>& vertices() const { return vertices_; }
  const vector<Edge>& edges() const { return edges_; }
  const vector<vector<VertexId>>& depth_map() const { return depth_map_; }
  bool has_vertex(const VertexId& vertex_id) const;
  bool is_connected(const VertexId& begin, const VertexId& end) const;
  Vertex& get_vertex(const VertexId& id);

 private:
  void add_id_to_depth_map(const VertexId& begin, const VertexId& end);
  VertexId num_of_vrt_ = 0;
  EdgeId num_of_edg_ = 0;

  VertexId next_vertex_id() { return num_of_vrt_++; }
  EdgeId next_edge_id() { return num_of_edg_++; }

  vector<Vertex> vertices_;
  vector<Edge> edges_;
  vector<vector<VertexId>> depth_map_;
};

Vertex& Graph::get_vertex(const VertexId& id) {
  for (auto& vertex : vertices_) {
    if (vertex.id == id)
      return vertex;
  }
  throw std::runtime_error("Vertex doesn't exist");
}

void Graph::add_id_to_depth_map(const VertexId& begin, const VertexId& end) {
  const size_t depth_for_new_vertex = get_vertex(begin).depth + 1;
  assert(!(depth_for_new_vertex > depth_map_.size()) &&
         "Error add id to depth map");
  if (depth_for_new_vertex == depth_map_.size()) {
    depth_map_.push_back({end});
  } else {
    depth_map_[depth_for_new_vertex].push_back(end);
  }
  get_vertex(end).depth = depth_for_new_vertex;
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

VertexId Graph::add_vertex() {
  auto vertex_id = next_vertex_id();
  vertices_.emplace_back(vertex_id);
  if (depth_map_.empty()) {
    depth_map_.push_back({vertex_id});
  }
  return vertex_id;
}

void Graph::add_edge(const VertexId& begin,
                     const VertexId& end,
                     const EdgeColor& color) {
  assert(has_vertex(begin) && "Vertex doesn't exist");
  assert(has_vertex(end) && "Vertex doesn't exist");
  assert(!(is_connected(begin, end)) && "Vertices already connected");
  const auto& edge = edges_.emplace_back(next_edge_id(), begin, end, color);
  get_vertex(begin).add_edge_id(edge.id);
  if (begin != end)
    get_vertex(end).add_edge_id(edge.id);
  if (color == EdgeColor::Gray)
    add_id_to_depth_map(begin, end);
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
bool to_be_or_not_to_be(int proba) {
  std::random_device rd;
  std::mt19937 mersenne(rd());
  return (int)(mersenne() % 100 + 1) <= proba;
}
void generate_gray_edges(int graph_depth,  //наибольшая возможная глубина
                         int new_vertices_num,
                         Graph& graph) {
  for (int depth = 0; depth < graph_depth - 1; ++depth) {
    if (depth >= graph.depth() + 1) {
      break;
    }
    int proba_gray = PROBA_GRAY_BEGIN - depth * PROBA_GRAY_BEGIN / graph_depth;
    for (const auto& vertex_id : graph.depth_map()[depth]) {
      for (int c = 0; c < new_vertices_num; ++c) {
        if (to_be_or_not_to_be(proba_gray)) {
          const auto new_vertex_id = graph.add_vertex();
          graph.add_edge(vertex_id, new_vertex_id, EdgeColor::Gray);
        }
      }
    }
  }
}
void generate_blue_edges(Graph& graph) {
  for (const auto& vector_ids : graph.depth_map()) {
    for (auto vertex_id_iter = vector_ids.begin();
         vertex_id_iter < vector_ids.end() - 1; ++vertex_id_iter) {
      if (to_be_or_not_to_be(PROBA_BLUE)) {
        graph.add_edge(*vertex_id_iter, *(vertex_id_iter + 1), EdgeColor::Blue);
      }
    }
  }
}

vector<VertexId> get_unconnected_vertex_ids(const vector<VertexId>& layer,
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

void generate_green_edges(Graph& graph) {
  for (auto& vertex : graph.vertices()) {
    if (to_be_or_not_to_be(PROBA_GREEN)) {
      graph.add_edge(vertex.id, vertex.id, EdgeColor::Green);
    }
  }
}

VertexId get_random_vertex_id(vector<VertexId> set_of_vertices_id) {
  std::random_device rd;
  std::mt19937 mersenne(rd());
  return set_of_vertices_id[mersenne() % set_of_vertices_id.size()];
}
void generate_yellow_edges(Graph& graph) {
  if (graph.depth() < 2)
    return;

  for (auto vertex_ids_at_depth = graph.depth_map().begin();
       vertex_ids_at_depth != graph.depth_map().end() - 1;
       ++vertex_ids_at_depth) {
    const int depth = graph.get_vertex((*vertex_ids_at_depth).front()).depth;
    const int proba_yellow = ((float)depth / (float)(graph.depth() - 1)) * 100;
    for (auto vertex_id = (*vertex_ids_at_depth).begin();
         vertex_id != (*vertex_ids_at_depth).end(); ++vertex_id) {
      if (to_be_or_not_to_be(proba_yellow)) {
        vector<VertexId> vertices_to_connect = get_unconnected_vertex_ids(
            *(vertex_ids_at_depth + 1), *vertex_id, graph);
        const int vertex_to_attach = get_random_vertex_id(vertices_to_connect);
        graph.add_edge(*vertex_id, vertex_to_attach, EdgeColor::Yellow);
      }
    }
  }
}

void generate_red_edges(Graph& graph) {
  if (graph.depth() < 3)
    return;

  for (auto vertex_ids_at_depth = graph.depth_map().begin();
       vertex_ids_at_depth != graph.depth_map().end() - 2;
       ++vertex_ids_at_depth) {
    for (auto vertex_id = (*vertex_ids_at_depth).begin();
         vertex_id != (*vertex_ids_at_depth).end(); ++vertex_id) {
      if (to_be_or_not_to_be(PROBA_RED)) {
        const int vertex_to_attach =
            get_random_vertex_id(*(vertex_ids_at_depth + 2));
        graph.add_edge(*vertex_id, vertex_to_attach, EdgeColor::Red);
      }
    }
  }
}

Graph generate_graph(int graph_depth, int new_vertices_num) {
  auto graph = Graph();

  //Генерация нулевой вершины:
  graph.add_vertex();

  generate_gray_edges(graph_depth, new_vertices_num, graph);
  generate_green_edges(graph);
  generate_yellow_edges(graph);
  generate_red_edges(graph);
  generate_blue_edges(graph);
  return graph;
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
  } while (new_vertices_num < 0);

  const Graph graph = generate_graph(depth, new_vertices_num);
  std::ofstream out("graph_task_03.json");
  out << graph;
  out.close();
  return 0;
}
