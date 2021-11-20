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

class Vertex {
 public:
  Vertex(const VertexId& id) : id_(id) {}

  // Возврат значений
  int get_depth() const { return depth_; }
  VertexId get_id() const { return id_; }
  const std::vector<EdgeId>& get_edge_ids() const { return edge_ids_; }

  // Добавить id ребра в edge_ids
  void add_edge_id(const EdgeId& id) {
    assert(!has_edge_id(id) && "This edge_id has already been added\n");
    edge_ids_.push_back(id);
  }

  // Выставить глубину вершины
  void set_depth(int depth) { depth_ = depth; }

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
  enum class Colors { Gray, Green, Blue, Yellow, Red };

  Edge(const VertexId& v1,
       const VertexId& v2,
       const EdgeId& id,
       int color_param = 0)
      : v1_(v1), v2_(v2), id_(id), color_param_(color_param) {}

  // Возврат значений
  EdgeId get_id() const { return id_; }
  VertexId get_vertex1_id() const { return v1_; }
  VertexId get_vertex2_id() const { return v2_; }

  std::string color_to_string() const {
    switch (color_param_) {
      case (int)Colors::Gray:
        return "gray";
      case (int)Colors::Green:
        return "green";
      case (int)Colors::Blue:
        return "blue";
      case (int)Colors::Yellow:
        return "yellow";
      case (int)Colors::Red:
        return "red";
    }
    return "gray";
  }

 private:
  EdgeId id_;
  VertexId v1_;
  VertexId v2_;
  int color_param_;
};

class Graph {
 public:
  // Возврат значений
  int get_depth() const { return depth_map_.size() - 1; }
  int get_vertices_cnt_in_depth(int depth) const {
    return depth_map_[depth].size();
  }

  // Добавляет ребро в graph
  void add_edge(const VertexId& v1, const VertexId& v2) {
    assert(has_vertex_id(v1) && "Vertex 1 doesnt exist\n");
    assert(has_vertex_id(v2) && "Vertex 2 doesnt exist\n");
    assert(!vertices_connected(v1, v2) && "Vertices are connected\n");

    int color_param = 0;
    // По логике нашей задачи, если вторая вершина не имеет присоединенных к ней
    // рёбер, значит добавляемое ребро серое
    if (get_vertex(v2).get_edge_ids().size() == 0) {
      // vertex2.depth = vertex1.depth + 1
      get_vertex(v2).set_depth(get_vertex(v1).get_depth() + 1);

      if (depth_map_.size() <= get_vertex(v2).get_depth()) {
        depth_map_.push_back({});
      }

      // Добавление на нужную глубину vertex2.id и удаление vertex2.id из
      // нулевой глубины
      depth_map_[get_vertex(v2).get_depth()].push_back(v2);
      for (auto vertex_id_it = depth_map_[0].begin();
           vertex_id_it != depth_map_[0].end(); vertex_id_it++) {
        if (*vertex_id_it == v2) {
          depth_map_[0].erase(vertex_id_it);
          break;
        }
      }
    } else {  // Иначе задаем цвет ребру
      color_param = set_edge_color(v1, v2);
    }
    const auto& new_edge =
        edges_.emplace_back(v1, v2, get_new_edge_id(), color_param);
    get_vertex(v1).add_edge_id(new_edge.get_id());
    if (v1 != v2) {  // Чтобы id зеленых рёбер не повторялись
      get_vertex(v2).add_edge_id(new_edge.get_id());
    }
  }

  // Добавляет вершину в граф
  void add_vertex() {
    if (depth_map_.size() == 0) {
      depth_map_.push_back({});
    }
    Vertex new_vertex = vertices_.emplace_back(get_new_vertex_id());
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
    for (auto& vertex : vertices_) {
      if (vertex.get_id() == id) {
        return vertex;
      }
    }
    return vertices_.back();
  }
  // Модификатор без const
  Vertex& get_vertex(const VertexId& id) {
    const auto& const_this = *this;
    return const_cast<Vertex&>(const_this.get_vertex(id));
  }

  // Проверяет, что вершины уже соединены
  bool vertices_connected(const VertexId& v1_id, const VertexId& v2_id) const {
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

  // Проверка, что вершина с таким id уже существует
  bool has_vertex_id(const VertexId& id) const {
    for (const auto& vertex : vertices_) {
      if (vertex.get_id() == id) {
        return true;
      }
    }
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
  int set_edge_color(const VertexId& v1, const VertexId& v2) {
    if (v1 == v2) {
      return 1;
    } else if (get_vertex(v1).get_depth() == get_vertex(v2).get_depth()) {
      return 2;
    } else if (get_vertex(v1).get_depth() == get_vertex(v2).get_depth() - 1) {
      return 3;
    } else if (get_vertex(v1).get_depth() == get_vertex(v2).get_depth() - 2) {
      return 4;
    }
    return 0;
  }
};

class GraphPrinter {
 public:
  std::string print(const Graph& graph) const {
    std::stringstream result;
    result << "{\n  \"depth\": " << graph.get_depth()
           << ",\n  \"vertices\": [\n    ";

    // Вывод вершин
    if (graph.get_vertices().size()) {
      result << print_vertex(graph.get_vertices()[0]);
      for (auto vertex_it = graph.get_vertices().begin() + 1;
           vertex_it != graph.get_vertices().end(); vertex_it++) {
        result << ",\n    " << print_vertex(*vertex_it);
      }
    }

    result << "\n  ],\n  \"edges\": [\n    ";

    // Вывод рёбер
    if (graph.get_edges().size()) {
      result << print_edge(graph.get_edges()[0]);
      for (auto edge_it = graph.get_edges().begin() + 1;
           edge_it != graph.get_edges().end(); edge_it++) {
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
    ss << "],\n      \"depth\": " << vertex.get_depth() << "\n    }";
    return ss.str();
  }

  std::string print_edge(const Edge& edge) const {
    std::stringstream ss;
    ss << "{\n      \"id\": " << edge.get_id() << ",\n      \"vertex_ids\": ["
       << edge.get_vertex1_id() << ", " << edge.get_vertex2_id()
       << "],\n      \"color\": \"" << edge.color_to_string() << "\"\n    }";
    return ss.str();
  }
};

// Генерация серых ребер
void generate_grey_edges(Graph& graph, int depth, int new_vertices_num) {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> edges_chance(0, 100);

  double percent = 100.0 / (double)depth;

  VertexId curr_vert_num = 0, added_vert_num = 1;

  graph.add_vertex();
  for (int curr_depth = 0; curr_depth <= depth; curr_depth++) {
    for (int curr_depth_vert_num = 0;
         curr_depth_vert_num < graph.get_vertices_cnt_in_depth(curr_depth);
         curr_depth_vert_num++) {
      for (int gen_vert_num = 0; gen_vert_num < new_vertices_num;
           gen_vert_num++) {
        if ((double)edges_chance(gen) > (double)curr_depth * percent) {
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
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> edges_chance(0, 100);

  for (const auto& vertex : graph.get_vertices()) {
    int green_edge_chance = 10;
    if (edges_chance(gen) < green_edge_chance) {
      graph.add_edge(vertex.get_id(), vertex.get_id());
    }
  }
}

// Генерация голубых ребер
void generate_blue_edges(Graph& graph) {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> edges_chance(0, 100);

  for (const auto& depth : graph.get_depth_map()) {
    for (auto vertex_id = depth.begin(); vertex_id != depth.end() - 1;
         vertex_id++) {
      int blue_edge_chance = 25;
      if (edges_chance(gen) < blue_edge_chance) {
        graph.add_edge(*vertex_id, *(vertex_id + 1));
      }
    }
  }
}

// Генерация желтых ребер
void generate_yellow_edges(Graph& graph, int depth) {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> edges_chance(0, 100);

  double yellow_edge_percent = 100.0 / (double)(depth - 1);
  int curr_depth = 0;
  for (auto depth = (graph.get_depth_map()).begin();
       depth != (graph.get_depth_map()).end() - 1; depth++) {
    for (auto& vertex_id : *depth) {
      // Вектор вершин уровнем глубже, которые не являются потомком j вершины
      std::vector<VertexId> vert_ids_depth_deeper;
      for (auto& vertex_id2 : *(depth + 1)) {
        if (!graph.vertices_connected(vertex_id, vertex_id2)) {
          vert_ids_depth_deeper.push_back(vertex_id2);
        }
      }
      // Проверка, что у нас есть "свободные" вершины для желтого ребра и рандом
      // выпал удачно для генерации
      if (vert_ids_depth_deeper.size() &&
          (double)edges_chance(gen) <
              yellow_edge_percent * (double)curr_depth) {
        // Выбираем вершину уровнем глубже, которая не является
        // потомком нашей вершины
        std::uniform_int_distribution<> rand_vertex(
            0, vert_ids_depth_deeper.size() - 1);
        int second_vert_num = 0;
        second_vert_num = rand_vertex(gen);

        graph.add_edge(vertex_id, vert_ids_depth_deeper[second_vert_num]);
      }
    }
    curr_depth++;
  }
}

// Генерация красных ребер
void generate_red_edges(Graph& graph, int depth) {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> edges_chance(0, 100);

  for (auto depth = graph.get_depth_map().begin();
       depth != graph.get_depth_map().end() - 2; depth++) {
    for (const auto& vertex_id : *depth) {
      double red_edge_chance = 33;
      if (edges_chance(gen) < red_edge_chance) {
        // Выбираем рандомом вершину 2мя уровнями глубже
        std::uniform_int_distribution<> rand_vertex(0, (depth + 2)->size() - 1);
        int second_vert_num = rand_vertex(gen);

        graph.add_edge(vertex_id, (*(depth + 2))[second_vert_num]);
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
  generate_yellow_edges(graph, depth);
  generate_red_edges(graph, depth);

  return graph;
}

int main() {
  int depth = 0;
  std::cout << "Input depth of graph:\n";
  std::cin >> depth;
  assert(depth >= 0 && "Depth must be >= 0");
  int new_vertices_num = 0;
  std::cout << "Input number of vertecies from each vertex:\n";
  std::cin >> new_vertices_num;
  assert(new_vertices_num >= 0 && "New_vertices_num must be >= 0");

  const auto graph = generate_graph(depth, new_vertices_num);

  const GraphPrinter output_graph;
  std::ofstream out_json("out.json");
  out_json << output_graph.print(graph);
  out_json.close();

  return 0;
}
