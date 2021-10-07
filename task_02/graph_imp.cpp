#include <fstream>
#include <iostream>
#include <vector>

using std::cout;
using std::endl;
using std::ofstream;
using std::vector;

int i = 0, j = 0;

struct E {
  int id;
  int v_s;
  int v_e;
};
struct V {
  int id;
  vector<int> e_id;
};

void create_mass_edges(E* e_g, int t[][2], int length);
void print_mass_edges(E* e_g, int length);
void create_mass_vertices(V* v_g, int max, E* e_g, int length);
void print_mass_vertices(V* v_g, int max);
void input_file_json(V* v_g, int max, E* e_g, int length);

int main() {
  // GRAPH
  int t[][2] = {
      {0, 1},  {0, 2},  {0, 3},  {1, 4},   {1, 5},   {1, 6},
      {2, 7},  {2, 8},  {3, 9},  {4, 10},  {5, 10},  {6, 10},
      {7, 11}, {8, 11}, {9, 12}, {10, 13}, {11, 13}, {12, 13},
  };

  // NUMBER OF GRAPH EDGES
  int length = sizeof(t) / sizeof(t[0]);

  // NUMBER OF GRAPH VERTICES
  int max = t[0][0];
  for (i = 0; i < length; i++) {
    for (j = 0; j < 2; j++) {
      if (t[i][j] > max)
        max = t[i][j];
    }
  }
  max++;

  E* e_g = new E[length];
  create_mass_edges(e_g, t, length);
  print_mass_edges(e_g, length);

  V* v_g = new V[max];
  create_mass_vertices(v_g, max, e_g, length);
  print_mass_vertices(v_g, max);

  input_file_json(v_g, max, e_g, length);

  delete[] v_g;
  v_g = 0;
  delete[] e_g;
  e_g = 0;

  return 0;
}

void create_mass_edges(E* e_g, int t[][2], int length) {
  for (i = 0; i < length; i++) {
    e_g[i].id = i;
    e_g[i].v_s = t[i][0];
    e_g[i].v_e = t[i][1];
  }
}

void print_mass_edges(E* e_g, int length) {
  cout << "EDGES: " << endl;
  for (i = 0; i < length; i++) {
    cout << e_g[i].id << " : " << e_g[i].v_s << " - " << e_g[i].v_e << endl;
  }
}

void create_mass_vertices(V* v_g, int max, E* e_g, int length) {
  for (i = 0; i < max; i++) {
    v_g[i].id = i;
    for (j = 0; j < length; j++) {
      if ((e_g[j].v_s == i) or (e_g[j].v_e == i)) {
        v_g[i].e_id.push_back(j);
      }
    }
  }
}

void print_mass_vertices(V* v_g, int max) {
  cout << endl << "VERTICES: " << endl;
  for (i = 0; i < max; i++) {
    cout << v_g[i].id << " : ";
    for (j = 0; j < v_g[i].e_id.size(); j++) {
      cout << v_g[i].e_id[j] << " ";
    }
    cout << endl;
  }
}

void input_file_json(V* v_g, int max, E* e_g, int length) {
  ofstream outf("graph_imp.json");
  outf << '{' << "\n\t" << '"' << "vertices" << '"' << ':' << " " << '['
       << "\n";
  for (i = 0; i < max - 1; i++) {
    outf << "\t\t" << '{' << '"' << "id" << '"' << ':' << v_g[i].id << ","
         << '"' << "edge_ids" << '"' << ':' << '[';
    for (j = 0; j < v_g[i].e_id.size() - 1; j++) {
      outf << v_g[i].e_id[j] << ",";
    }
    outf << v_g[i].e_id[v_g[i].e_id.size() - 1] << ']' << '}' << ",\n";
  }
  outf << "\t\t" << '{' << '"' << "id" << '"' << ':' << v_g[max - 1].id << ","
       << '"' << "edge_ids" << '"' << ':' << '[';
  for (j = 0; j < v_g[max - 1].e_id.size() - 1; j++) {
    outf << v_g[max - 1].e_id[j] << ",";
  }
  outf << v_g[max - 1].e_id[v_g[max - 1].e_id.size() - 1] << ']' << '}'
       << "\n\t" << ']' << ",\n\t";
  outf << '"' << "edges" << '"' << ':' << " " << '[' << "\n";
  for (i = 0; i < length - 1; i++) {
    outf << "\t\t" << '{' << '"' << "id" << '"' << ':' << e_g[i].id << ","
         << '"' << "vertex_ids" << '"' << ':' << '[';
    outf << e_g[i].v_s << ",";
    outf << e_g[i].v_e << ']' << '}' << ",\n";
  }
  outf << "\t\t" << '{' << '"' << "id" << '"' << ':' << v_g[length - 1].id
       << "," << '"' << "vertex_ids" << '"' << ':' << '[';
  outf << e_g[length - 1].v_s << ",";
  outf << e_g[length - 1].v_e << ']' << '}' << "\n";
  outf << "\t" << ']' << "\n";
  outf << "}";
}
