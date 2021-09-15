#include <iostream>
#include <vector>
#include <fstream>

using std::cout;
using std::endl;
using std::vector;

//using namespace std;

struct Vertex {
    int id;
    int output_edg_num;

    Vertex(): id(-1), output_edg_num(-1) {}
    Vertex(int iid): id(iid), output_edg_num(0) {}
    Vertex(int id, int edg): id(id), output_edg_num(edg) {}
};

struct Edge {
    int id;
    int node[2];

    Edge(int start, int end, int id) {
        node[0] = start;
        node[1] = end;
        this->id = id;
    }
};

class Graph {
//privat
int size, depth;
vector<Vertex> vertices;
vector<Edge> edges;

public:
    Graph(): size(0) {}

    Graph(const vector<Edge> & init_edges, const vector<Vertex> & init_vertices) {
        vertices = init_vertices;
        edges = init_edges;
        size = vertices.size();
    }

    ~Graph() {
        vertices.clear();
        edges.clear();
    }

    int write_json() {
        std::ofstream out;
        out.open("graph.json", std::ofstream::out | std::ofstream::trunc);
        out << "{\n\t\"vertices\": [\n\t\t";
        for (int i = 0; i < size; i++) {
            out << "{\n\t\t\t\"id\": " << i << ",\n\t\t\t\"edge_ids\": [";
            int cnt(0);
            for (vector<Edge>::iterator it = edges.begin(); it != edges.end(); it++) {
                if ((*it).node[0] == i) {
                    cnt++;
                    if (cnt < vertices[i].output_edg_num) {
                        out << it->id << ", ";
                    } else out << (*it).id;
                }
            }
            out << "]\n\t\t}";
            if (i != (size-1)) out << ", ";
        }
        out << "\n\t],\n\t\"edges\": [\n\t\t";
        for(auto it = edges.begin(); it != edges.end(); it++) {
            out << "{\n\t\t\t\"id\": " << it->id << ",\n\t\t\t\"vertex_ids\": [" << it->node[0] << ", " << it->node[1] << "]\n\t\t}";
            if (it != edges.end()-1) {
                out << ", ";
            }
        }
        out << "\n\t]\n}";
        out.close();
        return 0;
    }

};

int main(int argc, char**argv) {
    vector<Edge> init_edges = { {0,1,0}, {0,2,1}, {0,3,2}, {1,4,3}, {1,5,4}, {1,6,5}, {2,7,6}, {2,8,7},
                            {3,9,8}, {4,10,9}, {5,10,10}, {6,10,11}, {7,11,12}, {8,11,13}, {9,12,14},
                            {10,13,15}, {11,13,16}, {12,13,17} };

    vector<Vertex> init_vertices = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13};

    for (auto it = init_edges.begin(); it != init_edges.end(); it++) {
        init_vertices[(*it).node[0]].output_edg_num++;
    }

    Graph A(init_edges, init_vertices);
    A.write_json();

    return 0;
}
