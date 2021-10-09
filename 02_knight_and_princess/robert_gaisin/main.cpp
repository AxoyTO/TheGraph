#include <ctime>
#include <fstream>
#include <iostream>
#include <vector>

using std::cin;
using std::cout;
using std::endl;
using std::vector;

using VertexId = int;
using EdgeId = int;

const int Grey = 0;
const int Green = 1;
const int Blue = 2;
const int Yellow = 3;
const int Red = 4;

//Перегрузка вывода вектора из int
std::ostream& operator<<(std::ostream& out, const vector<int>& c) {
  if (c.size() > 0) {
    out << c[0];
    for (auto i = begin(c) + 1; i != end(c); ++i)
      out << ", " << *i;
  }
  return out;
}
//Структура для описания вершины графа
struct Vertex {
  VertexId id = 0;
  int depth = 0;
  vector<EdgeId> edge_ids;
};
//Перегрузка вывода для вектора из вершин
std::ostream& operator<<(std::ostream& out, const Vertex& vertex) {
  out << "{" << endl;
  out << "      \"id\": " << vertex.id << "," << endl;
  out << "      \"edge_ids\": [" << vertex.edge_ids << "]" << endl;
  out << "      \"depth\": " << vertex.depth << endl;
  out << "    }";
  return out;
}
//Структура для описания ребра графа
struct Edge {
  EdgeId id = 0;
  VertexId begin = 0;
  VertexId end = 0;
  int color = 0;
};
//Перегрузка вывода для вектора из ребер
std::ostream& operator<<(std::ostream& out, const Edge& edge) {
  out << "{" << endl;
  out << "      \"id\": " << edge.id << "," << endl;
  out << "      \"vertex_ids\": [" << edge.begin << ", " << edge.end << "]"
      << endl;
  out << "      \"color\": ";
  switch (edge.color) {
    case Grey:
      out << "\"grey\"";
      break;
    case Green:
      out << "\"green\"";
      break;
    case Blue:
      out << "\"blue\"";
      break;
    case Yellow:
      out << "\"yellow\"";
      break;
    case Red:
      out << "\"red\"";
      break;
  }
  out << endl;
  out << "    }";
  return out;
}
//Класс для описания графа
class Graph {
 public:
  //Изменение размера атрибута класса, в котором хранятся все вершины графа
  void resize(int graph_depth) { vertices_.resize(graph_depth); }
  //Методы для получения следующего идентефикатора для вершин и ребер
  VertexId next_vertex_id() { return num_of_vrt_++; }
  EdgeId next_edge_id() { return num_of_edg_++; }
  //Следующие методы добавляют:
  //новую вершину в граф
  void add_vertex(Vertex& vertex, int layer);
  //новое ребро в граф
  void add_edge(Edge& edge) { edges_.push_back(edge); }
  //идентефикатор ребра к существующей вершине графа
  void add_edge_in_vertex(int layer, int vertex_in_layer, EdgeId edge_id);
  //Метод проверяет слой графа на существование или отсутствие в нем вершин
  bool layer_is_empty_or_not_exist(int layer);
  //Метод возвращает идентификатор для вершины слоя layer находящейся на позиции
  //в этом слое vertex_in_layer
  VertexId get_vertex_id(const int& layer, const int& vertex_in_layer) const;
  //Метод возвращает число вершин на глубине layer графа
  size_t layer_size(int layer);
  //Константные методы:
  //возвращает число слоев (общую глубину) графа
  const int depth() const { return depth_; }
  //возвращает контейнер со всеми верщинами графа
  const vector<vector<Vertex>> vertices() const { return vertices_; }
  //возвращает контейнер с ребрами
  const vector<Edge> edges() const { return edges_; }

 private:
  int depth_ = 0;
  VertexId num_of_vrt_ = 0;
  EdgeId num_of_edg_ = 0;
  vector<vector<Vertex>> vertices_;
  vector<Edge> edges_;
};

void Graph::add_vertex(Vertex& vertex, int layer) {
  if (layer >= depth_) {
    depth_ = layer + 1;
  }
  vertices_[layer].push_back(vertex);
}

void Graph::add_edge_in_vertex(int layer, int vertex_in_layer, EdgeId edge_id) {
  vertices_[layer][vertex_in_layer].edge_ids.push_back(edge_id);
}

bool Graph::layer_is_empty_or_not_exist(int layer) {
  return (layer >= depth_) || vertices_[layer].empty();
}

VertexId Graph::get_vertex_id(const int& layer,
                              const int& vertex_in_layer) const {
  return vertices_[layer][vertex_in_layer].id;
}

size_t Graph::layer_size(int layer) {
  if (layer_is_empty_or_not_exist(layer)) {
    return 0;
  }
  return vertices_[layer].size();
}
//Перегрузка вывода для вектора из вершин (используется для вывод отдельного
//слоя)
std::ostream& operator<<(std::ostream& out, const vector<Vertex>& c) {
  if (c.size() > 0) {
    out << c[0];
    for (auto i = begin(c) + 1; i != end(c); ++i)
      out << ", " << *i;
  }
  return out;
}
//Перегрузка вывода для вектора векторов из вершин (вывод всех имеющихся вершин)
std::ostream& operator<<(std::ostream& out, const vector<vector<Vertex>>& c) {
  if (c.size() > 0) {
    out << c[0];
    for (auto i = begin(c) + 1; i != end(c); ++i)
      out << ", " << *i;
  }
  return out;
}
//Вспомогательная функция для выбора соседа (вершины или слоя), с которым нужно
//соединиться ребром
int neighbour_choice(int rule, size_t k, size_t sub) {
  switch (rule) {
    case 1:  //есть только сосед на расстоянии sub с меньшим индексом
      return k - sub;
    case 2:  //есть только сосед на расстоянии sub с большим индексом
      return k + sub;
    case 3:  //есть оба соседа, подкидывем монетку, к кому кидать ребро
      const int coin = rand() % 2;
      return coin * (k - sub) + (1 - coin) * (k + sub);
  }
  return -1;
}
//Функция генератор графов для задачи 3
Graph graph_generator(int graph_depth,
                      int vrtx_on_first_lvl,
                      int new_vertices_num) {
  Graph returned_graph;
  returned_graph.resize(graph_depth);
  //Генерация вершин первого слоя:
  for (int i = 0; i < vrtx_on_first_lvl; ++i) {
    Vertex x = {returned_graph.next_vertex_id(), 0};
    returned_graph.add_vertex(x, 0);
  }
  //Вероятность порождения вершины следующего слоя:
  int p = 100;
  //Настолько вероятность будет уменьшена для следующего слоя:
  int sub_p = p / graph_depth;
  //Генерация вершин начиная со второго слоя:
  for (int i = 0; i < graph_depth - 1; ++i) {
    if (returned_graph.layer_is_empty_or_not_exist(i)) {
      break;
    }
    for (size_t k = 0; k < returned_graph.layer_size(i); ++k) {
      for (int c = 0; c < new_vertices_num; ++c) {
        if (rand() % 100 + 1 <= p) {
          Vertex x = {returned_graph.next_vertex_id(), i + 1};
          Edge y = {returned_graph.next_edge_id(),
                    returned_graph.get_vertex_id(i, k), x.id, Grey};
          returned_graph.add_edge(y);
          x.edge_ids.push_back(y.id);
          returned_graph.add_edge_in_vertex(i, k, y.id);
          returned_graph.add_vertex(x, i + 1);
        }
      }
    }
    p -= sub_p;  //Уменьшили вероятность для следующего слоя
  }  //Граф сгенерирован, все ребра в нем серые
  //Значения вероятостей возникновения ребер соответствующих цветов:
  const int p_green = 10;
  const int p_blue = 25;
  int p_yellow = 0;
  const int p_red = 33;
  //Настолько будет увеличиваться от слоя к слою вероятность возникновения
  //желтого ребра:
  int add_p_yellow = 100 / returned_graph.depth();
  //Обходим граф, для каждой вершины проверяем вероятность возникновения
  //соответствующих ребер и добавляем их к графу:
  for (int i = 0; i < returned_graph.depth(); ++i) {
    for (size_t k = 0; k < returned_graph.layer_size(i); ++k) {
      if (rand() % 100 + 1 <= p_green) {
        VertexId tmp = returned_graph.get_vertex_id(i, k);
        Edge y = {returned_graph.next_edge_id(), tmp, tmp, Green};
        returned_graph.add_edge(y);
        returned_graph.add_edge_in_vertex(i, k, y.id);
      }
      if (rand() % 100 + 1 <= p_blue) {
        // rule - рещающее правило для передачи в neighbour_choice(), будет
        // равно 0 - если вершина одна в слое и соседа нет, 1 - если есть сосед в
        // этом же слое с меньшим номером, 2 - если есть сосед в этом же слое с
        // большим номером, 3 - если есть оба соседа:
        const int rule =
            (int)(k > 0) + 2 * (int)(k < returned_graph.layer_size(i) - 1);
        if (rule) {
          const int neighbour = neighbour_choice(rule, k, 1);

          Edge y = {returned_graph.next_edge_id(),
                    returned_graph.get_vertex_id(i, k),
                    returned_graph.get_vertex_id(i, neighbour), Blue};
          returned_graph.add_edge(y);
          returned_graph.add_edge_in_vertex(i, k, y.id);
          returned_graph.add_edge_in_vertex(i, neighbour, y.id);
        }
      }
      if (k == returned_graph.layer_size(i) -
                   1)  //Дабы ошибки округления не сильно влияли на результат
        p_yellow = 100;
      if (rand() % 100 + 1 <= p_yellow) {
        //По аналогии с предыдущим случаем, только работает не для вершин, а для
        //слоев:
        const int rule =
            (int)(i > 0) + 2 * (int)(i < returned_graph.depth() - 1);
        if (rule) {
          const int neighbour_layer = neighbour_choice(rule, i, 1);
          const int vertex_to_attach =
              rand() % returned_graph.layer_size(neighbour_layer);
          Edge y = {
              returned_graph.next_edge_id(), returned_graph.get_vertex_id(i, k),
              returned_graph.get_vertex_id(neighbour_layer, vertex_to_attach),
              Yellow};
          returned_graph.add_edge(y);
          returned_graph.add_edge_in_vertex(i, k, y.id);
          returned_graph.add_edge_in_vertex(neighbour_layer, vertex_to_attach,
                                            y.id);
        }
      }
      if (rand() % 100 + 1 <= p_red) {
        //Все аналогично предыдущему, только проверяется наличие слоев
        //находящиеся от текущего через один:
        const int rule =
            (int)(i > 1) + 2 * (int)(i < returned_graph.depth() - 2);
        if (rule) {
          const int neighbour_layer = neighbour_choice(rule, i, 2);
          const int vertex_to_attach =
              rand() % returned_graph.layer_size(neighbour_layer);
          Edge y = {
              returned_graph.next_edge_id(), returned_graph.get_vertex_id(i, k),
              returned_graph.get_vertex_id(neighbour_layer, vertex_to_attach),
              Red};
          returned_graph.add_edge(y);
          returned_graph.add_edge_in_vertex(i, k, y.id);
          returned_graph.add_edge_in_vertex(neighbour_layer, vertex_to_attach,
                                            y.id);
        }
      }
    }
    p_yellow += add_p_yellow;  //Увеличили вероятность появления желтых ребер
                               //для следующего слоя
  }
  return returned_graph;
}
//Перегрузка вывода для вектора из ребер
std::ostream& operator<<(std::ostream& out, const vector<Edge>& c) {
  if (c.size() > 0) {
    out << c[0];
    for (auto i = begin(c) + 1; i != end(c); ++i)
      out << ", " << *i;
  }
  return out;
}
//Перегрузка вывода для графа
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

int main() {
  cout << "Enter graph depth: ";
  int depth;
  cin >> depth;
  cout << "Enter new vertices number: ";
  int new_vertices_num;
  cin >> new_vertices_num;
  srand(time(0));
  const Graph graph = graph_generator(depth, 1, new_vertices_num);
  std::ofstream out("graph_task_02.json");
  out << graph;
  out.close();
  return 0;
}
