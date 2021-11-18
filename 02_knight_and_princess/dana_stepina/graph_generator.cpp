#include "graph.hpp"

//ГЕНЕРАЦИЯ ГРАФА
//Шанс создания vertices_count вершин на данной глубине
int check_number_new_vertices(const VertexId& vertices_count,
                              const int& percent) {
  if (rand() % 100 <= percent)
    return vertices_count;
  else if (vertices_count == 0)
    return 0;
  else
    return check_number_new_vertices(vertices_count - 1, percent);
}
//Генератор графа
void generate_graph(Graph& graph,
                    const DepthGraph& depth = 0,
                    const VertexId& vertices_count = 0) {
  int percent_current_depth;
  VertexId count_new_vertices;
  //Нулевая вершина
  graph.add_vertex();

  for (DepthGraph current_depth = 0; current_depth <= depth; current_depth++) {
    //Процент генерации вершины на текущей глубине
    percent_current_depth = floor(100 * (depth - current_depth) / depth);
    //Вектор из вершин текущей глубины
    std::vector<Vertex> vertices_in_current_depth;
    for (const auto& vertex : graph.get_vertices())
      if (vertex.depth == current_depth)
        vertices_in_current_depth.push_back(vertex);

    //Проверка - есть ли вершины на текущей глубине
    if (current_depth != depth) {
      if (vertices_in_current_depth.empty()) {
      graph.decrease_graph_depth();
      cout << "The depth of the generated graph is less than expected."
              "\nDepth of the generated graph: "
           << graph.get_graph_depth() << endl;
      break;
      }
      graph.increase_graph_depth();
    }

    //Создание новых вершин для каждой из вершин текущей глублины
    for (const auto& vertex : vertices_in_current_depth) {
      count_new_vertices =
          check_number_new_vertices(vertices_count, percent_current_depth);
      VertexId last_new_vertex_id =
          graph.get_vertex_id_counter() - 1 + count_new_vertices;

      for (VertexId new_vertex_id = graph.get_vertices().back().id + 1;
           new_vertex_id <= last_new_vertex_id; new_vertex_id++) {
        graph.add_vertex();
        graph.add_edge(vertex.id, new_vertex_id);
      }
    }
  }
}
