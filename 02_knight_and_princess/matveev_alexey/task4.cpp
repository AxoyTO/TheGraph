#include <cassert>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iostream>
#include <vector>

using VertId = int;
using EdgeId = int;

enum class Colors { gray, green, blue, yellow, red };

struct Vertex {
  VertId id;
  int depth;
  std::vector<EdgeId> edges;
};

struct Edge {
  EdgeId id;
  Colors color;
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
    new_vert.depth = 0;
    verts.push_back(new_vert);
    vert_num++;
  }

  void AddEdge(VertId vertex1, VertId vertex2, int pre_gen) {
    assert(!CheckNeighborhood(vertex1, vertex2) &&
           "These vertexes are already connected");
    Edge new_edge;
    new_edge.id = edge_num;
    if (vertex1 > vertex2) {
      vertex1 ^= vertex2;
      vertex2 ^= vertex1;
      vertex1 ^= vertex2;
    }
    new_edge.verts[0] = vertex1;
    new_edge.verts[1] = vertex2;
    if (pre_gen) {
      new_edge.color = Colors::gray;
      verts[new_edge.verts[1]].depth = verts[new_edge.verts[0]].depth + 1;
    } else if (vertex1 == vertex2) {
      new_edge.color = Colors::green;
    } else if (verts[vertex1].depth == verts[vertex2].depth) {
      new_edge.color = Colors::blue;
    } else if (verts[vertex2].depth - verts[vertex1].depth == 1) {
      new_edge.color = Colors::yellow;
    } else {
      new_edge.color = Colors::red;
    }
    edges.push_back(new_edge);
    verts[vertex1].edges.push_back(edge_num);
    verts[vertex2].edges.push_back(edge_num);
    edge_num++;
  }

  ~Graph() {}
};

class GraphPrinter {
 public:
  GraphPrinter() {}
  GraphPrinter(Graph g) {
    std::cout << '{' << std::endl << "  \"vertices\": [" << std::endl << "    ";
    for (auto it1 = g.verts.begin(); it1 != g.verts.end(); it1++) {
      std::cout << '{' << std::endl
                << "      \"id\": " << (*it1).id << ',' << std::endl;
      auto it2 = (*it1).edges.begin();
      std::cout << "      \"edge_ids\": [" << *it2;
      it2++;
      for (; it2 != (*it1).edges.end(); it2++) {
        std::cout << ", " << *it2;
      }
      std::cout << "]," << std::endl
                << "      \"depth\": " << (*it1).depth << std::endl
                << "    }, ";
    }
    std::cout << std::endl << "  ]," << std::endl;
    std::cout << "  \"edges\": [" << std::endl << "    ";
    for (auto it = g.edges.begin(); it != g.edges.end(); it++) {
      std::cout << '{' << std::endl
                << "      \"id\": " << (*it).id << ',' << std::endl;
      std::cout << "      \"vertex_ids\": [" << (*it).verts[0] << ", ";
      std::cout << (*it).verts[1] << "]," << std::endl;
      std::cout << "      \"color\": ";
      PrintColor((*it).color);
      std::cout << "    }, ";
    }
    std::cout << std::endl << "  ]" << std::endl << '}' << std::endl;
  }

 private:
  void PrintColor(Colors color) {
    switch (color) {
      case Colors::gray:
        std::cout << "\"gray\"" << std::endl;
        break;
      case Colors::green:
        std::cout << "\"green\"" << std::endl;
        break;
      case Colors::blue:
        std::cout << "\"blue\"" << std::endl;
        break;
      case Colors::yellow:
        std::cout << "\"yellow\"" << std::endl;
        break;
      case Colors::red:
        std::cout << "\"red\"" << std::endl;
        break;
    }
  }
};

bool RandValue(double probability) {
  return static_cast<double>(rand() % 100) < probability;
}

int main() {
  Graph g;
  int depth, new_vertices_num;
  std::cout << "Enter depth:" << std::endl;
  std::cin >> depth;
  while (depth < 0) {
    std::cout << "Depth must be not negative. Enter depth again:" << std::endl;
    std::cin >> depth;
  }
  std::cout << "Enter number of new vertices:" << std::endl;
  std::cin >> new_vertices_num;
  while (depth < 0) {
    std::cout << "Number of new vertices must be not negative. Enter number of "
                 "new vertices again:"
              << std::endl;
    std::cin >> new_vertices_num;
  }
  double step = 100.0 / static_cast<double>(depth);
  srand(time(NULL));
  g.AddVert();
  VertId parent = 0;
  int cur_depth = 0, verts_on_cur_depth, verts_on_prev_depth = 1;
  for (; cur_depth < depth && verts_on_prev_depth > 0; cur_depth++) {
    verts_on_cur_depth = 0;
    for (int i = verts_on_prev_depth; i > 0; i--) {
      for (int j = 0; j < new_vertices_num; j++) {
        if (RandValue(100.0 - step * static_cast<double>(cur_depth))) {
          g.AddVert();
          g.AddEdge(g.vert_num - i - 1 - verts_on_cur_depth, g.vert_num - 1, 1);
          verts_on_cur_depth++;
        }
      }
    }
    verts_on_prev_depth = verts_on_cur_depth;
  }
  if (cur_depth < depth) {
    std::cout << "Depth of the graph is less than given. Depth is " << cur_depth
              << std::endl;
  }
  for (int i = 0; i < g.vert_num - 1; i++) {
    if (RandValue(10)) {
      g.AddEdge(i, i, 0);
    }
    if (RandValue(25) && g.verts[i + 1].depth == g.verts[i].depth) {
      g.AddEdge(i, i + 1, 0);
    }
    if (RandValue(100.0 * g.verts[i].depth / (cur_depth - 1)) &&
        g.verts[i].depth < cur_depth) {
      int next_level_num = 0;
      std::vector<VertId> next_level;
      for (int j = i; j < g.vert_num; j++) {
        if (g.verts[j].depth == g.verts[i].depth + 1 &&
            !g.CheckNeighborhood(i, j)) {
          next_level.push_back(j);
          next_level_num++;
        }
      }
      g.AddEdge(i, next_level[rand() % next_level_num], 0);
    }
    if (RandValue(33) && g.verts[i].depth < cur_depth - 1) {
      int after_next_level_first = g.vert_num, after_next_level_num = 0;
      for (int j = i; j < g.vert_num; j++) {
        if (g.verts[j].depth == g.verts[i].depth + 2) {
          if (j < after_next_level_first) {
            after_next_level_first = j;
          }
          after_next_level_num++;
        }
      }
      g.AddEdge(i, after_next_level_first + rand() % after_next_level_num, 0);
    }
  }
  if (RandValue(10)) {
    g.AddEdge(g.vert_num - 1, g.vert_num - 1, 0);
  }

  std::streambuf* buf;
  std::ofstream file;
  file.open("graph.json");
  buf = std::cout.rdbuf();
  std::cout.rdbuf(file.rdbuf());
  GraphPrinter prt = GraphPrinter(g);
  std::cout.rdbuf(buf);
  return 0;
}
