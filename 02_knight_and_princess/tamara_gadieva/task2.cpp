#include "task2.hpp"

void Vertex::json_string(std::ofstream& graph_json) const {
  graph_json << "\t\t{ \"id\": " << this->id << ", \"edge_ids\": [";
  for (int i = 0; i < (int)(this->edges.size()); i++) {
    graph_json << this->edges[i];
    if (i != (int)(this->edges.size() - 1))
      graph_json << ", ";
    else
      graph_json << "] }";
  }
}

void Edge::json_string(std::ofstream& graph_json) const {
  graph_json << "\t\t{ \"id\": " << this->id << ", \"vertex_ids\": ["
             << this->vertex_1 << ", " << this->vertex_2 << "] }";
}

void print_graph(std::vector<Vertex> vertex,
                 int vertices_num,
                 std::vector<Edge> edge,
                 int edges_num) {
  std::ofstream graph_json("graph.json");

  graph_json << "{" << std::endl;

  graph_json << "\t\"vertices\": [" << std::endl;
  for (int i = 0; i < vertices_num; i++) {
    vertex[i].json_string(graph_json);
    if (i != vertices_num - 1)
      graph_json << "," << std::endl;
    else
      graph_json << std::endl << "\t]," << std::endl;
  }

  graph_json << "\t\"edges\": [" << std::endl;
  for (int i = 0; i < edges_num; i++) {
    edge[i].json_string(graph_json);
    if (i != edges_num - 1)
      graph_json << "," << std::endl;
    else
      graph_json << std::endl << "\t]" << std::endl;
  }

  graph_json << "}" << std::endl;

  graph_json.close();
}

int main() {
  const int vertices_num = 14;
  const int edges_num = 18;

  std::vector<Vertex> vertex;
  std::vector<Edge> edge;

  vertex.resize(vertices_num);
  for (int i = 0; i < vertices_num; i++) {
    vertex[i].id = i;
  }

  edge.resize(edges_num);
  for (int i = 0; i < edges_num; i++) {
    edge[i].id = i;
  }

  //информация о парах вершин, соединённых рёбрами
  std::vector<std::pair<int, int>> vertex_pairs = {
      {0, 1},  {0, 2},  {0, 3},  {1, 4},   {1, 5},   {1, 6},
      {2, 7},  {2, 8},  {3, 9},  {4, 10},  {5, 10},  {6, 10},
      {7, 11}, {8, 11}, {9, 12}, {10, 13}, {11, 13}, {12, 13}};

  for (int i = 0; i < (int)vertex_pairs.size(); i++) {
    //заполняем номера рёбер для вершин графа
    vertex[vertex_pairs[i].first].edges.push_back(i);
    vertex[vertex_pairs[i].second].edges.push_back(i);
    //заполняет номера вершин графа для рёбер
    edge[i].vertex_1 = vertex_pairs[i].first;
    edge[i].vertex_2 = vertex_pairs[i].second;
  }

  print_graph(vertex, vertices_num, edge, edges_num);

  return 0;
}