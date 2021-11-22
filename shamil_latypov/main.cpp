#include <assert.h>
#include <cassert>
#include <fstream>
#include <iostream>
#include <random>
#include <sstream>
#include <string>
#include <vector>

using VertexId = int;
using EdgeId = int;
constexpr int GREEN_EDGE_CHANCE = 10;
constexpr int BLUE_EDGE_CHANCE = 25;
constexpr int RED_EDGE_CHANCE = 33;
constexpr int DEPTH_MIN = 0;
constexpr int NEW_VERTICES_MIN = 0;

class Vertex {
 public:
  explicit Vertex(const VertexId& id) : id_(id) {}

  int depth;
  // Возврат значений
  VertexId get_id() const { return id_; }
  const std::vector<EdgeId>& get_edge_ids() const { return edge_ids_; }

  // Добавить id ребра в edge_ids
  void add_edge_id(const EdgeId& id) {
    assert(!has_edge_id(id) && "This edge_id has already been added\n");
    edge_ids_.push_back(id);
  }

  // Связана ли вершина с ребром
  bool has_edge_id(const EdgeId& id) const {
    for (const auto& edge_id : edge_ids_) {
      if (edge_id == id) {
        return true;
      }
    }
    return false;
  }

 private:
  int depth_;
  VertexId id_;
  std::vector<EdgeId> edge_ids_;
};

class Edge {
 public:
  enum class Color { Gray, Green, Blue, Yellow, Red };

  Edge(const VertexId& v1,
       const VertexId& v2,
       const EdgeId& id,
       const Color& color)
      : v1_(v1), v2_(v2), id_(id), color_(color) {}

  // Возврат значений
  Color get_color() const { return color_; }
  EdgeId get_id() const { return id_; }
  VertexId get_vertex1_id() const { return v1_; }
  VertexId get_vertex2_id() const { return v2_; }

 private:
  EdgeId id_;
  VertexId v1_;
  VertexId v2_;
  Color color_;
};

class Graph {
 public:
  // Возврат значений
  int get_depth() const { return depth_map_.size() - 1; }
  const std::vector<VertexId>& get_vertices_in_depth(int depth) const {
    return depth_map_[depth];
  }

  // Добавляет ребро в graph
  void add_edge(const VertexId& vertex1_id, const VertexId& vertex2_id) {
    assert(has_vertex_id(vertex1_id) && "Vertex 1 doesnt exist");
    assert(has_vertex_id(vertex2_id) && "Vertex 2 doesnt exist");
    assert(!vertices_connected(vertex1_id, vertex2_id) &&
           "Vertices are connected");

    auto& vertex1 = get_vertex(vertex1_id);
    auto& vertex2 = get_vertex(vertex2_id);
    const auto& color = calculate_edge_color(vertex1, vertex2);

    const auto& new_edge =
        edges_.emplace_back(vertex1_id, vertex2_id, get_new_edge_id(), color);
    vertex1.add_edge_id(new_edge.get_id());
    if (color != Edge::Color::Green) {
      vertex2.add_edge_id(new_edge.get_id());
    }
    if (color == Edge::Color::Gray) {
      update_vertex_depth(vertex1, vertex2);
    }
  }

  // Добавляет вершину в граф
  void add_vertex() {
    if (depth_map_.size() == 0) {
      depth_map_.push_back({});
    }
    const Vertex& new_vertex = vertices_.emplace_back(get_new_vertex_id());
    depth_map_[0].push_back(new_vertex.get_id());
  }

  // Вектор вершин
  const std::vector<Vertex>& get_vertices() const { return vertices_; }
  // Вектор рёбер
  const std::vector<Edge>& get_edges() const { return edges_; }
  // Вектор карты глубины
  const std::vector<std::vector<VertexId>>& get_depth_map() const {
    return depth_map_;
  }

  // Возврат вершины
  const Vertex& get_vertex(const VertexId& id) const {
    assert(has_vertex_id(id) && "Vertex doesnt exist");
    for (auto& vertex : vertices_) {
      if (vertex.get_id() == id) {
        return vertex;
      }
    }
    throw std::runtime_error("Vertex doesn't exist");
  }
  // Модификатор без const
  Vertex& get_vertex(const VertexId& id) {
    const auto& const_this = *this;
    return const_cast<Vertex&>(const_this.get_vertex(id));
  }

  // Проверка, что вершина с таким id уже существует
  bool has_vertex_id(const VertexId& id) const {
    for (const auto& vertex : vertices_) {
      if (vertex.get_id() == id) {
        return true;
      }
    }
    return false;
  }

  // Проверяет, что вершины уже соединены
  bool vertices_connected(const VertexId& v1_id, const VertexId& v2_id) const {
    assert(has_vertex_id(v1_id) && "Vertex 1 doesnt exist");
    assert(has_vertex_id(v2_id) && "Vertex 2 doesnt exist");
    // Если вершины разные, то проверяются edge_ids обеих вершин
    if (v1_id != v2_id) {
      for (const auto& edge_id1 : get_vertex(v1_id).get_edge_ids()) {
        for (const auto& edge_id2 : get_vertex(v2_id).get_edge_ids()) {
          if (edge_id1 == edge_id2) {
            return true;
          }
        }
      }
    } else {  // Иначе ищется ребро, которое ведет из вершины в саму себя
      for (const auto& edge : edges_) {
        if (edge.get_vertex1_id() == v1_id && edge.get_vertex2_id() == v2_id) {
          return true;
        }
      }
    }  // Иначе вершины не соединены
    return false;
  }

 private:
  VertexId vert_num_ = 0;
  EdgeId edge_num_ = 0;
  std::vector<Vertex> vertices_;
  std::vector<Edge> edges_;
  std::vector<std::vector<VertexId>> depth_map_;

  VertexId get_new_vertex_id() { return vert_num_++; }
  EdgeId get_new_edge_id() { return edge_num_++; }

  // Выставить цвет ребра
  // Поставил в private, тк не вижу логики вызывать эту функцию извне
  Edge::Color calculate_edge_color(const Vertex& vertex1,
                                   const Vertex& vertex2) const {
    if (vertex2.get_edge_ids().size() == 0) {
      return Edge::Color::Gray;
    } else if (vertex1.get_id() == vertex2.get_id()) {
      return Edge::Color::Green;
    } else if (vertex1.depth == vertex2.depth) {
      return Edge::Color::Blue;
    } else if (vertex1.depth == vertex2.depth - 1) {
      return Edge::Color::Yellow;
    } else if (vertex1.depth == vertex2.depth - 2) {
      return Edge::Color::Red;
    }
    throw std::runtime_error("Failed to determine edge color");
  }

  void update_vertex_depth(const Vertex& vertex1, Vertex& vertex2) {
    vertex2.depth = vertex1.depth + 1;

    if (depth_map_.size() <= vertex2.depth) {
      depth_map_.push_back({});
    }

    // Добавление на нужную глубину vertex2.id и удаление vertex2.id из
    // нулевой глубины
    depth_map_[vertex2.depth].push_back(vertex2.get_id());
    for (auto vertex_id_it = depth_map_[0].begin();
         vertex_id_it != depth_map_[0].end(); vertex_id_it++) {
      if (*vertex_id_it == vertex2.get_id()) {
        depth_map_[0].erase(vertex_id_it);
        break;
      }
    }
  }
};

std::string color_to_string(Edge::Color color) {
  switch (color) {
    case Edge::Color::Gray:
      return "gray";
    case Edge::Color::Green:
      return "green";
    case Edge::Color::Blue:
      return "blue";
    case Edge::Color::Yellow:
      return "yellow";
    case Edge::Color::Red:
      return "red";
  }
  throw std::runtime_error("Invalid color value");
}

class GraphPrinter {
 public:
  std::string print(const Graph& graph) const {
    std::stringstream result;
    result << "{\n  \"depth\": " << graph.get_depth()
           << ",\n  \"vertices\": [\n    ";

    // Вывод вершин
    const auto& vertices = graph.get_vertices();
    if (vertices.size()) {
      result << print_vertex(vertices[0]);
      for (auto vertex_it = vertices.begin() + 1; vertex_it != vertices.end();
           vertex_it++) {
        result << ",\n    " << print_vertex(*vertex_it);
      }
    }

    result << "\n  ],\n  \"edges\": [\n    ";

    // Вывод рёбер
    const auto& edges = graph.get_edges();
    if (edges.size()) {
      result << print_edge(edges[0]);
      for (auto edge_it = edges.begin() + 1; edge_it != edges.end();
           edge_it++) {
        result << ",\n    " << print_edge(*edge_it);
      }
    }

    result << "\n  ]\n}\n";

    return result.str();
  }

  std::string print_vertex(const Vertex& vertex) const {
    std::stringstream ss;
    ss << "{\n      \"id\": " << vertex.get_id() << ",\n      \"edge_ids\": [";

    if (vertex.get_edge_ids().size()) {
      ss << vertex.get_edge_ids()[0];
      for (auto edge_id_it = vertex.get_edge_ids().begin() + 1;
           edge_id_it != vertex.get_edge_ids().end(); edge_id_it++) {
        ss << ", " << *edge_id_it;
      }
    }
    ss << "],\n      \"depth\": " << vertex.depth << "\n    }";
    return ss.str();
  }

  std::string print_edge(const Edge& edge) const {
    std::stringstream ss;
    ss << "{\n      \"id\": " << edge.get_id() << ",\n      \"vertex_ids\": ["
       << edge.get_vertex1_id() << ", " << edge.get_vertex2_id()
       << "],\n      \"color\": \"" << color_to_string(edge.get_color())
       << "\"\n    }";
    return ss.str();
  }
};

int random_number() {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> random_num(0, 100);

  return random_num(gen);
}

VertexId get_random_vertex_id(const std::vector<VertexId>& vertices_ids) {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> random_vertex(0, vertices_ids.size() - 1);

  return vertices_ids[random_vertex(gen)];
}

// Генерация серых ребер
void generate_grey_edges(Graph& graph, int depth, int new_vertices_num) {
  double percent = 100.0 / (double)depth;

  VertexId curr_vert_num = 0, added_vert_num = 1;

  graph.add_vertex();
  for (int curr_depth = 0; curr_depth <= depth; curr_depth++) {
    for (int curr_depth_vert_num = 0;
         curr_depth_vert_num < graph.get_vertices_in_depth(curr_depth).size();
         curr_depth_vert_num++) {
      for (int gen_vert_num = 0; gen_vert_num < new_vertices_num;
           gen_vert_num++) {
        if ((double)random_number() > (double)curr_depth * percent) {
          graph.add_vertex();

          graph.add_edge(curr_vert_num, added_vert_num);

          added_vert_num++;
        }
      }
      curr_vert_num++;
    }
  }
}

// Генерация зеленых ребер
void generate_green_edges(Graph& graph) {
  for (const auto& vertex : graph.get_vertices()) {
    if (random_number() < GREEN_EDGE_CHANCE) {
      graph.add_edge(vertex.get_id(), vertex.get_id());
    }
  }
}

// Генерация голубых ребер
void generate_blue_edges(Graph& graph) {
  for (const auto& depth : graph.get_depth_map()) {
    for (auto vertex_id = depth.begin(); vertex_id != depth.end() - 1;
         vertex_id++) {
      if (random_number() < BLUE_EDGE_CHANCE) {
        graph.add_edge(*vertex_id, *(vertex_id + 1));
      }
    }
  }
}

// Генерация желтых ребер
void generate_yellow_edges(Graph& graph) {
  double yellow_edge_percent = 100.0 / (double)(graph.get_depth() - 1);
  for (auto depth = (graph.get_depth_map()).begin();
       depth != (graph.get_depth_map()).end() - 1; depth++) {
    for (const auto& vertex_id : *depth) {
      // Вектор вершин уровнем глубже, которые не являются потомком j вершины
      std::vector<VertexId> vert_ids_depth_deeper;
      for (const auto& vertex_id2 : *(depth + 1)) {
        if (!graph.vertices_connected(vertex_id, vertex_id2)) {
          vert_ids_depth_deeper.push_back(vertex_id2);
        }
      }
      // Проверка, что у нас есть "свободные" вершины для желтого ребра и рандом
      // выпал удачно для генерации
      if (vert_ids_depth_deeper.size() &&
          (double)random_number() <
              yellow_edge_percent *
                  (double)(depth - graph.get_depth_map().begin())) {
        graph.add_edge(vertex_id, get_random_vertex_id(vert_ids_depth_deeper));
      }
    }
  }
}

// Генерация красных ребер
void generate_red_edges(Graph& graph) {
  for (auto depth = graph.get_depth_map().begin();
       depth != graph.get_depth_map().end() - 2; depth++) {
    for (const auto& vertex_id : *depth) {
      if (random_number() < RED_EDGE_CHANCE) {
        // Выбираем рандомом вершину 2мя уровнями глубже
        graph.add_edge(vertex_id, get_random_vertex_id(*(depth + 2)));
      }
    }
  }
}

// Генерация графа
Graph generate_graph(int depth, int new_vertices_num) {
  Graph graph;

  generate_grey_edges(graph, depth, new_vertices_num);
  generate_green_edges(graph);
  generate_blue_edges(graph);
  generate_yellow_edges(graph);
  generate_red_edges(graph);

  return graph;
}

int main() {
  int depth = 0;
  do {
    std::cout << "Input depth of graph:\n";
    std::cin >> depth;
    if (depth < DEPTH_MIN)
      std::cout << "Depth must be >= 0\n";
  } while (depth < DEPTH_MIN);
  int new_vertices_num = 0;
  do {
    std::cout << "Input number of vertecies from each vertex:\n";
    std::cin >> new_vertices_num;
    if (new_vertices_num < NEW_VERTICES_MIN)
      std::cout << "New_vertices_num must be >= 0\n";
  } while (new_vertices_num < NEW_VERTICES_MIN);

  const auto graph = generate_graph(depth, new_vertices_num);

  const GraphPrinter output_graph;
  std::ofstream out_json("out.json");
  out_json << output_graph.print(graph);
  out_json.close();

  return 0;
}
