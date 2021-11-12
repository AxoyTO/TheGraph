#include <array>
#include <fstream>
#include <iostream>
#include <vector>
using std::vector;
constexpr int num_of_vertices = 14;
constexpr int num_of_edges = 18;
constexpr int all_edges =
    num_of_vertices * (num_of_vertices - 1) / 2 + num_of_vertices;
vector<vector<int>> vertices = {
    {0, 1, 2},       {0, 3, 4, 5}, {1, 6, 7}, {2, 8},      {3, 9},
    {4, 10},         {5, 11},      {6, 12},   {7, 13},     {8, 14},
    {9, 10, 11, 15}, {12, 13, 16}, {14, 17},  {15, 16, 17}};
vector<vector<int>> edges = {{0, 1},   {0, 2},   {0, 3},  {1, 4},  {1, 5},
                             {1, 6},   {2, 7},   {2, 8},  {3, 9},  {4, 10},
                             {5, 10},  {6, 10},  {7, 11}, {8, 11}, {9, 12},
                             {10, 13}, {11, 13}, {12, 13}};

struct VertexStruct {
  int id;
  vector<int> edgeIds;
};
struct EdgeStruct {
  int id;
  vector<int> vertexIds;
};

void print_vertices(VertexStruct v, std::ofstream& file) {
  file << "{\n\t\t\t\"id\": " << v.id << ",\n\t\t\t\"edge_ids\": [";
  for (auto i = v.edgeIds.begin(); i != v.edgeIds.end(); i++) {
    int index = std::distance(v.edgeIds.begin(), i);
    if (*i != v.edgeIds.back()) {
      file << *i << ",";
    } else {
      file << *i;
    }
  }
  file << "]\n\t\t}";
}
void print_edges(EdgeStruct e, std::ofstream& file) {
  file << "{\n\t\t\t\"id\": " << e.id << ",\n\t\t\t\"vertex_ids\": [";
  for (auto i = e.vertexIds.begin(); i != e.vertexIds.end(); i++) {
    int index = std::distance(e.vertexIds.begin(), i);
    if (*i != e.vertexIds.back()) {
      file << *i << ",";
    } else {
      file << *i;
    }
  }
  file << "]\n\t\t}";
}
void print_graph(VertexStruct v[], EdgeStruct e[], std::ofstream& file) {
  file << "{\n\t\"vertices\": [\n\t\t";
  for (int i = 0; i < num_of_vertices; i++) {
    print_vertices(v[i], file);
    if (i != num_of_vertices - 1) {
      file << ",\n\t\t";
    } else {
      file << "\n\t";
    }
  }
  file << "],\n\t\"edges\": [\n\t\t";
  for (int i = 0; i < num_of_edges; i++) {
    print_edges(e[i], file);
    if (i != num_of_edges - 1) {
      file << ",\n\t\t";
    } else {
      file << "\n\t";
    }
  }
  file << "]\n}";
}

int main() {
  VertexStruct verticesArray[num_of_vertices];
  EdgeStruct edgesArray[num_of_edges];

  for (auto it = vertices.begin(); it != vertices.end(); ++it) {
    int index = std::distance(vertices.begin(), it);
    VertexStruct newVertexStruct;
    newVertexStruct.id = index;
    newVertexStruct.edgeIds = *it;
    verticesArray[index] = newVertexStruct;
  }

  for (auto it = edges.begin(); it != edges.end(); ++it) {
    int index = std::distance(edges.begin(), it);
    EdgeStruct newEdgeStruct;
    newEdgeStruct.id = index;
    newEdgeStruct.vertexIds = *it;
    edgesArray[index] = newEdgeStruct;
  }

  std::ofstream graphOutput;
  graphOutput.open("graph.json");
  print_graph(verticesArray, edgesArray, graphOutput);
}
