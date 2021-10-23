#include <algorithm>
#include <cassert>
#include <fstream>
#include <iostream>
#include <vector>

using VertId = int;
using EdgeId = int;

struct Vertex {
  VertId id;
  std::vector<EdgeId> edges;
};

struct Edge {
  EdgeId id;
  VertId verts[2];
};

class Graph {
 public:
  std::vector<Vertex> verts;
  std::vector<Edge> edges;
  int vert_num, edge_num;

  Graph() { vert_num = edge_num = 0; }

  bool CheckNeighborhood(VertId vertex1, VertId vertex2) {
    assert(std::max(vertex1, vertex2) <= vert_num &&
           "Vertex index is out of range");
    if (vertex1 != vertex2) {
      for (auto it1 = verts[vertex1].edges.begin();
           it1 != verts[vertex1].edges.end(); it1++) {
        for (auto it2 = verts[vertex2].edges.begin();
             it2 != verts[vertex2].edges.end(); it2++) {
          if (*it1 == *it2) {
            return true;
          }
        }
      }
      return false;
    }
    for (auto it = verts[vertex1].edges.begin();
         it != verts[vertex1].edges.end(); it++) {
      if (edges[*it].verts[0] == edges[*it].verts[1]) {
        return true;
      }
    }
    return false;
  }

  void AddVert() {
    Vertex new_vert;
    new_vert.id = vert_num;
    verts.push_back(new_vert);
    vert_num++;
  }

  void AddEdge(VertId vertex1, VertId vertex2) {
    assert(!CheckNeighborhood(vertex1, vertex2) &&
           "These vertexes are already connected");
    Edge new_edge;
    new_edge.id = edge_num;
    new_edge.verts[0] = std::min(vertex1, vertex2);
    new_edge.verts[1] = std::max(vertex1, vertex2);
    edges.push_back(new_edge);
    verts[vertex1].edges.push_back(edge_num);
    verts[vertex2].edges.push_back(edge_num);
    edge_num++;
  }

  ~Graph() {}
};

int main() {
  VertId vrt[18][2] = {{0, 1},  {0, 2},  {0, 3},  {1, 4},   {1, 5},   {1, 6},
                       {2, 7},  {2, 8},  {3, 9},  {4, 10},  {5, 10},  {6, 10},
                       {7, 11}, {8, 11}, {9, 12}, {10, 13}, {11, 13}, {12, 13}};
  Graph g;
  for (int i = 0; i < 14; i++) {
    g.AddVert();
  }
  for (int i = 0; i < 18; i++) {
    g.AddEdge(vrt[i][0], vrt[i][1]);
  }
  return 0;
}
