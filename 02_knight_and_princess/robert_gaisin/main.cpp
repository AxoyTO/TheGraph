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

const int Gray = 0;
const int Green = 1;
const int Blue = 2;
const int Yellow = 3;
const int Red = 4;

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
  const VertexId id;
  size_t depth;
  vector<EdgeId> edge_ids;
};

//Перегрузка вывода для вектора из вершин
std::ostream& operator<<(std::ostream& out, const Vertex& vertex) {
  out << "{" << endl;
  out << "      \"id\": " << vertex.id << "," << endl;
  out << "      \"edge_ids\": [" << vertex.edge_ids << "]," << endl;
  out << "      \"depth\": " << vertex.depth << endl;
  out << "}";
  return out;
}

struct Edge {
  const EdgeId id;
  VertexId begin = 0;
  VertexId end = 0;
  int color = 0;
};

//Перегрузка вывода для ребра
std::ostream& operator<<(std::ostream& out, const Edge& edge) {
  out << "{" << endl;
  out << "      \"id\": " << edge.id << "," << endl;
  out << "      \"vertex_ids\": [" << edge.begin << ", " << edge.end << "],"
      << endl;
  out << "      \"color\": ";
  switch (edge.color) {
    case Gray:
      out << "\"gray\"";
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
  out << "}";
  return out;
}

class Graph {
 public:
  void add_vertex(size_t depth);

  void add_edge(const VertexId& begin, const VertexId& end, int color);

  int depth() const { return depth_sizes_.size(); }
  int depth_size(int depth) { return depth_sizes_[depth]; }
  int number_of_vertices() { return num_of_vrt_; }
  const vector<Vertex>& vertices() const { return vertices_; }
  const vector<Edge>& edges() const { return edges_; }

 private:
  VertexId num_of_vrt_ = 0;
  EdgeId num_of_edg_ = 0;

  VertexId next_vertex_id() { return num_of_vrt_++; }
  EdgeId next_edge_id() { return num_of_edg_++; }
  bool connection(const VertexId& begin, const VertexId& end);
  //вершины
  vector<Vertex> vertices_;
  //ребра
  vector<Edge> edges_;
  //размеры слоев
  vector<int> depth_sizes_;
};

bool Graph::connection(const VertexId& begin, const VertexId& end) {
  bool ret = false;
  for (const EdgeId& edge_num : vertices_[begin].edge_ids) {
    if ((ret = edges_[edge_num].begin == end || edges_[edge_num].end == end)) {
      break;
    }
  }
  return ret;
}

void Graph::add_vertex(size_t depth) {
  Vertex vertex = {next_vertex_id(), depth};
  vertices_.push_back(vertex);
  if (depth >= depth_sizes_.size())
    for (size_t i = depth_sizes_.size(); i <= depth; ++i)
      depth_sizes_.push_back(0);
  ++depth_sizes_[depth];
}

void Graph::add_edge(const VertexId& begin, const VertexId& end, int color) {
  if ((begin < num_of_vrt_) && (end < num_of_vrt_) && !connection(begin, end)) {
    Edge edge = {next_edge_id(), begin, end, color};
    vertices_[begin].edge_ids.push_back(edge.id);
    vertices_[end].edge_ids.push_back(edge.id);
    edges_.push_back(edge);
  }
}

std::ostream& operator<<(std::ostream& out, const vector<Vertex>& vertices) {
  for (auto it = vertices.begin(); it != vertices.end(); ++it) {
    out << *it;
    if (it != vertices.end() - 1) {
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

  //Генерация вершин первого слоя:
  for (int i = 0; i < vrtx_on_first_lvl; ++i) {
    //    Vertex x = {returned_graph.next_vertex_id(), 0};
    returned_graph.add_vertex(0);
  }
  //Вероятность порождения вершины следующего слоя:
  int p = 100;
  //Настолько вероятность будет уменьшена для следующего слоя:
  int sub_p = p / graph_depth;
  //Генерация вершин начиная со второго слоя:
  size_t offset = 0;
  size_t offset_plus = 0;
  //Вектор смещений слоев графа, локальная область видимости, т.к. вообще говоря
  //вершины из разных слоев не обязательно должны идти подряд, но конкретно в
  //этом генераторе это гарантируется
  vector<size_t> offsets;
  for (int i = 0; i < graph_depth - 1; ++i) {
    if (i >= returned_graph.depth()) {
      break;
    }
    offsets.push_back(offset);
    offset_plus += returned_graph.depth_size(i);
    for (size_t k = offset; k < offset_plus; ++k) {
      for (int c = 0; c < new_vertices_num; ++c) {
        if (rand() % 100 + 1 <= p) {
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
  //Граф сгенерирован, все ребра в нем серые
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
    for (size_t k = offsets[i]; k < offsets[i + 1]; ++k) {
      if (rand() % 100 + 1 <= p_green) {
        returned_graph.add_edge(k, k, Green);
      }
      if (rand() % 100 + 1 <= p_blue) {
        // rule - рещающее правило для передачи в neighbour_choice(), будет
        //равно 0, если вершина одна в слое и соседа нет, 1 - если есть сосед в
        //этом же слое с меньшим номером, 2 - если есть сосед в этом же слое с
        //большим номером, 3 - если есть оба соседа:
        const int rule =
            (int)(k > offsets[i]) + 2 * (int)(k < offsets[i + 1] - 1);
        if (rule) {
          const int neighbour = neighbour_choice(rule, k, 1);
          returned_graph.add_edge(k, neighbour, Blue);
        }
      }
      if (i == returned_graph.depth() - 1)
        p_yellow = 100;  //Дабы ошибки округления не сильно влияли на результат
                         //на последнем слое вероятность появления желтого ребра
                         //гарантировано равна 100%
      if (rand() % 100 + 1 <= p_yellow) {
        //По аналогии с предыдущим случаем, только работает не для вершин, а для
        //слоев:
        const int rule =
            (int)(i > 0) + 2 * (int)(i < returned_graph.depth() - 1);
        if (rule) {
          const int neighbour_layer = neighbour_choice(rule, i, 1);
          const int vertex_to_attach =
              offsets[neighbour_layer] +
              rand() % returned_graph.depth_size(neighbour_layer);
          returned_graph.add_edge(k, vertex_to_attach, Yellow);
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
              offsets[neighbour_layer] +
              rand() % returned_graph.depth_size(neighbour_layer);
          returned_graph.add_edge(k, vertex_to_attach, Red);
        }
      }
    }
    //Увеличили вероятность появления желтых ребер
    //для следующего слоя
    p_yellow += add_p_yellow;
  }
  return returned_graph;
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
  std::ofstream out("graph_task_03.json");
  out << graph;
  out.close();
  return 0;
}
