#include <assert.h>
#include <algorithm>
#include <cassert>
#include <chrono>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <random>
#include <sstream>
#include <string>
#include <vector>

using VertexId = int;
using EdgeId = int;

class Vertex {
 public:
  Vertex(int depth, const VertexId& id) : depth_(depth), id_(id) {}

  // Возврат значений
  int get_depth() const { return depth_; }
  VertexId get_id() const { return id_; }
  const std::vector<EdgeId>& get_edge_ids() const { return edge_ids_; }

  // Добавить id ребра в edge_ids
  void add_edge_id(const EdgeId& id) {
    assert(has_edge_id(id) && "This edge_id has already been added\n");
    edge_ids_.push_back(id);
  }

  // Добавить id сына в son_vert_ids
  void add_son_vertex_id(const VertexId& id) {
    assert(has_son_vert_id(id) && "This vertex_id has already been added\n");
    son_vert_ids_.push_back(id);
  }

  bool is_connected(const VertexId& id) const {
    for (auto& vertex_id : son_vert_ids_) {
      if (vertex_id == id) {
        return true;
      }
    }
    return false;
  }

  bool has_edge_id(const EdgeId& id) const {
    for (const auto& edge_id : edge_ids_) {
      if (edge_id == id) {
        return false;
      }
    }
    return true;
  }

  bool has_son_vert_id(const VertexId& id) const {
    for (const auto& vertex_id : son_vert_ids_) {
      if (vertex_id == id) {
        return false;
      }
    }
    return true;
  }

 private:
  int depth_;
  VertexId id_;
  std::vector<EdgeId> edge_ids_;
  std::vector<VertexId> son_vert_ids_;
};

class Edge {
 public:
  Edge(const VertexId& v1,
       const VertexId& v2,
       const EdgeId& id,
       int color_param = 0)
      : v1_(v1), v2_(v2), id_(id), color_param_(color_param) {}

  // Возврат значений
  int get_color() const { return color_param_; }
  EdgeId get_id() const { return id_; }
  VertexId get_vertex1_id() const { return v1_; }
  VertexId get_vertex2_id() const { return v2_; }

 private:
  EdgeId id_;
  VertexId v1_;
  VertexId v2_;
  int color_param_;
};

class Graph {
 public:
  void set_depth(int depth) { depth_ = depth; }

  // Возврат значений
  int get_depth() const { return depth_; }
  int get_vertices_cnt_in_depth(int depth) const {
    return vertices_ids_[depth].size();
  }

  // Добавляет ребро в graph
  void add_edge(const VertexId& v1, const VertexId& v2, int color_param = 0) {
    assert(has_vertex_id(v1) && "Vertex 1 doesnt exist\n");
    assert(has_vertex_id(v2) && "Vertex 2 doesnt exist\n");
    assert(vertices_connected(v1, v2) && "Vertices are connected\n");
    const auto& new_edge =
        edges_.emplace_back(v1, v2, get_new_edge_id(), color_param);
    vertices_[v1].add_edge_id(new_edge.get_id());
    if (v1 != v2) {
      vertices_[v2].add_edge_id(new_edge.get_id());
      add_son_in_vert(v1, v2);
    }
  }

  // Добавляет вершину в граф
  void add_vertex() {
    if (vertices_ids_.size() <= depth_) {
      vertices_ids_.resize(depth_ + 1);
    }
    Vertex new_vertex = vertices_.emplace_back(depth_, get_new_vertex_id());
    vertices_ids_[depth_].push_back(new_vertex.get_id());
  }

  // Возврат векторов
  const std::vector<Vertex>& get_vertices() const { return vertices_; }
  const std::vector<Edge>& get_edges() const { return edges_; }
  const std::vector<std::vector<VertexId>>& get_vertices_ids() const {
    return vertices_ids_;
  }

  bool vertices_connected(const VertexId& v1, const VertexId& v2) const {
    if (v1 != v2) {
      for (const auto& edge_id1 : vertices_[v1].get_edge_ids()) {
        for (const auto& edge_id2 : vertices_[v2].get_edge_ids()) {
          if (edge_id1 == edge_id2) {
            return false;
          }
        }
      }
    }
    return true;
  }

  bool has_vertex_id(const VertexId& id) const {
    for (const auto& vertex_id : vertices_) {
      if (vertex_id.get_id() == id) {
        return true;
      }
    }
    return false;
  }

 private:
  int depth_ = 0;
  VertexId vert_num_ = 0;
  EdgeId edge_num_ = 0;
  std::vector<Vertex> vertices_;
  std::vector<Edge> edges_;
  std::vector<std::vector<VertexId>> vertices_ids_;

  VertexId get_new_vertex_id() { return vert_num_++; }
  EdgeId get_new_edge_id() { return edge_num_++; }

  void add_son_in_vert(const VertexId& v1, const VertexId& v2) {
    assert(has_vertex_id(v1) && "Vertex v1 doesn't exist\n");
    assert(has_vertex_id(v2) && "Vertex v2 doesn't exist\n");
    vertices_[v1].add_son_vertex_id(v2);
  }
};

class GraphPrinter {
 public:
  std::string print(const Graph& graph) const {
    std::stringstream result;
    result << "{\n  \"depth\": " << graph.get_depth()
           << ",\n  \"vertices\": [\n    ";
    bool check_first_comma = false;

    for (const auto& vertex : graph.get_vertices()) {
      if (check_first_comma) {
        result << ",\n    ";
      }

      result << print_vertex(vertex);
      check_first_comma = true;
    }

    result << "\n  ],\n  \"edges\": [\n    ";
    check_first_comma = false;

    for (const auto& edge : graph.get_edges()) {
      if (check_first_comma) {
        result << ",\n    ";
      }

      result << print_edge(edge);
      check_first_comma = true;
    }

    result << "\n  ]\n}\n";

    return result.str();
  }

  std::string print_vertex(const Vertex& vertex) const {
    std::stringstream ss;
    ss << "{\n      \"id\": " << vertex.get_id() << ",\n      \"edge_ids\": [";
    int check_first_comma = false;
    for (auto& i : vertex.get_edge_ids()) {
      if (check_first_comma) {
        ss << ", ";
      }
      check_first_comma = true;
      ss << i;
    }
    ss << "],\n      \"depth\": " << vertex.get_depth() << "\n    }";
    return ss.str();
  }

  std::string print_edge(const Edge& edge) const {
    std::stringstream ss;
    std::vector<std::string> colors = {"gray", "green", "blue", "yellow",
                                       "red"};
    ss << "{\n      \"id\": " << edge.get_id() << ",\n      \"vertex_ids\": ["
       << edge.get_vertex1_id() << ", " << edge.get_vertex2_id()
       << "],\n      \"color\": \"" << colors[edge.get_color()] << "\"\n    }";
    return ss.str();
  }
};

// Генерация графа
Graph generate_graph(int depth, int new_vertices_num) {
  // Генерирующая штука из примера
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> edges_chance(0, 100);

  Graph graph;

  int color_param = 0;
  int max_depth = 0;
  double percent = 100.0 / (double)depth;

  VertexId curr_vert_num = 0, added_vert_num = 1;

  // Генерация серых ребер
  graph.add_vertex();
  for (int curr_depth = 0; curr_depth <= depth; curr_depth++) {
    for (int curr_depth_vert_num = 0;
         curr_depth_vert_num < graph.get_vertices_cnt_in_depth(curr_depth);
         curr_depth_vert_num++) {
      for (int gen_vert_num = 0; gen_vert_num < new_vertices_num;
           gen_vert_num++) {
        if ((double)edges_chance(gen) > (double)curr_depth * percent) {
          max_depth = curr_depth + 1;
          graph.set_depth(max_depth);

          // Добавляем новую вершину в graph
          graph.add_vertex();

          // Добавляем новое ребро в graph
          graph.add_edge(curr_vert_num, added_vert_num, color_param);

          added_vert_num++;
        }
      }
      curr_vert_num++;
    }
  }

  color_param++;
  // Генерация зеленых ребер
  for (const auto& vertex : graph.get_vertices()) {
    int green_edge_chance = 10;
    if (edges_chance(gen) < green_edge_chance) {
      // Добавляем новое ребро в graph
      graph.add_edge(vertex.get_id(), vertex.get_id(), color_param);
    }
  }

  color_param++;
  // Генерация голубых ребер
  for (const auto& depth : graph.get_vertices_ids()) {
    for (auto vertex_id = depth.begin(); vertex_id != depth.end() - 1;
         vertex_id++) {
      int blue_edge_chance = 25;
      if (edges_chance(gen) < blue_edge_chance) {
        graph.add_edge(*vertex_id, *(vertex_id + 1), color_param);
      }
    }
  }

  color_param++;
  // Генерация желтых ребер
  double yellow_edge_percent = 100.0 / (double)(depth - 1);
  int curr_depth = 0;
  for (auto depth = (graph.get_vertices_ids()).begin();
       depth != (graph.get_vertices_ids()).end() - 1; depth++) {
    for (auto& vertex_id : *depth) {
      // Вектор вершин уровнем глубже, которые не являются потомком j вершины
      std::vector<VertexId> vert_ids_depth_deeper;
      for (auto& vertex_id2 : *(depth + 1)) {
        if (!graph.get_vertices()[vertex_id].is_connected(vertex_id2)) {
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

        graph.add_edge(vertex_id, vert_ids_depth_deeper[second_vert_num],
                       color_param);
      }
    }
    curr_depth++;
  }

  color_param++;
  // Генерация красных ребер
  for (auto depth = graph.get_vertices_ids().begin();
       depth != graph.get_vertices_ids().end() - 2; depth++) {
    for (const auto& vertex_id : *depth) {
      double red_edge_chance = 33;
      if (edges_chance(gen) < red_edge_chance) {
        // Выбираем рандомом вершину 2мя уровнями глубже
        std::uniform_int_distribution<> rand_vertex(0, (depth + 2)->size() - 1);
        int second_vert_num = rand_vertex(gen);

        graph.add_edge(vertex_id, (*(depth + 2))[second_vert_num], color_param);
      }
    }
  }

  return graph;
}

int main() {
  int depth = 0;
  std::cout << "Input depth of graph:\n";
  std::cin >> depth;
  int new_vertices_num = 0;
  std::cout << "Input number of vertecies from each vertex:\n";
  std::cin >> new_vertices_num;

  Graph graph;
  graph = generate_graph(depth, new_vertices_num);

  GraphPrinter output_graph;
  std::ofstream out_json("out.json");
  out_json << output_graph.print(graph);
  out_json.close();

  return 0;
}
