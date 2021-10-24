#include <fstream>
#include <iostream>
// using namespace std;
const int num_of_vertices = 14;
const int num_of_edges = 18;
const int all_edges =
    num_of_vertices * (num_of_vertices - 1) / 2 + num_of_vertices;

struct Vertex {
  int id;
  int edge_ids[all_edges];
};
struct Edge {
  int id;
  int vertex_ids[2];
};

int ARRAY_SIZE_V(Vertex x[]) {
  return (*(&x + 1) - x);
}
int ARRAY_SIZE_E(Edge x[]) {
  return (*(&x + 1) - x);
}

void Vertex_output(Vertex v, std::ofstream& file) {
  file << "{\n\t\t\t\"id\": " << v.id << ",\n\t\t\t\"edge_ids\": [";
  for (int i = 0; i < std::size(v.edge_ids); i++) {
    if (v.edge_ids[i] > -1) {
      file << v.edge_ids[i] << ",";
    } else {
      break;
    }
  }
  file << "],\n\t\t}";
}
void Edge_output(Edge e, std::ofstream& file) {
  file << "{\n\t\t\t\"id\": " << e.id << ",\n\t\t\t\"vertex_ids\": [";
  for (int i = 0; i < std::size(e.vertex_ids); i++) {
    if (e.vertex_ids[i] > -1) {
      file << e.vertex_ids[i] << ",";
    } else {
      break;
    }
  }
  file << "],\n\t\t}";
}
void Graph_output(Vertex v[], Edge e[], std::ofstream& file) {
  file << "{\n\t\"vertices\": [\n\t\t";
  for (int i = 0; i < ARRAY_SIZE_V(v); i++) {
    if (v[i].id > -1) {
      Vertex_output(v[i], file);
      if (i != (ARRAY_SIZE_V(v) - 1)) {
        file << ",\n\t\t";
      }
    } else {
      break;
    }
  }
  file << "],\n\t\"edges\": [\n\t\t";
  for (int i = 0; i < ARRAY_SIZE_E(e); i++) {
    if (e[i].id > -1) {
      Edge_output(e[i], file);
      if (i != (ARRAY_SIZE_E(e) - 1)) {
        file << ",\n\t\t";
      }
    } else {
      break;
    }
  }
  file << "]\n}";
}

int main() {
  // input
  std::ifstream verin("vertices.txt");
  std::ifstream edgin("edges.txt");
  Vertex v[num_of_vertices];
  Edge e[num_of_edges];
  int index;
  int current_v, current_e, incr = 0;
  while (!verin.eof()) {
    while (verin >> current_v) {
      verin >> index;
      v[current_v].id = current_v;
      for (int i = 0; i < all_edges; i++) {
        if (i < index) {
          verin >> v[current_v].edge_ids[i];
        } else {
          v[current_v].edge_ids[i] = -1;
        }
      }
      incr++;
    }
  }
  std::cout << "vertices" << std::endl;
  for (int i = 0; i < incr; i++) {
    std::cout << i << ": ";
    for (int j = 0; j < 4; j++) {
      if (v[i].edge_ids[j] > -1) {
        std::cout << (v[i].edge_ids[j]) << " ";
      }
    }
    std::cout << "\n";
  }
  incr = 0;
  while (!edgin.eof()) {
    edgin >> current_e;
    e[current_e].id = current_e;
    edgin >> e[current_e].vertex_ids[0];
    edgin >> e[current_e].vertex_ids[1];
    incr++;
  }
  std::cout << "edges" << std::endl;
  for (int i = 0; i < incr; i++) {
    std::cout << i << ": ";
    for (int j = 0; j < 2; j++) {
      if (e[i].vertex_ids[j] > -1) {
        std::cout << (e[i].vertex_ids[j]) << " ";
      }
    }
    std::cout << "\n";
  }
  // generate simplest graph
  // for (int i = 0; i < num_of_edges; i++)
  //{
  //    for (int j = 0; j < num_of_vertices; j++)
  //    {
  //
  //    }
  //}

  // output as .json

  std::ofstream gra;
  gra.open("graph.json");
  Graph_output(v, e, gra);

  // graph.close();
}