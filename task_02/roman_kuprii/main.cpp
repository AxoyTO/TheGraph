#include <iostream>
#include <vector>
#include <fstream>
#include <array>

using std::cout;
using std::endl;
using std::vector;

//using namespace std;

struct Vertex {
    int id;
    int edges;

    Vertex(int _id = -1, int edg = 0): id(_id), edges(edg) {}
};

struct Edge {
//TODO check uniqueness of ID field
    int id;
    std::array<int, 2> nodes;

    Edge(int start, int end, int _id): id(_id) {
        nodes[0] = start;
        nodes[1] = end;
    }
};

class Graph {
    int size_;
    vector<Vertex> vertices_;
    vector<Edge> edges_;

public:
    Graph(): size_(0) {}

    Graph(const vector<Edge> & init_edges, const vector<Vertex> & init_vertices) {
        vertices_ = init_vertices;
        edges_ = init_edges;
        size_ = vertices_.size();
    }

//TODO: remove this function from Graph's ownership
    int write_json() {
        std::ofstream out;
        out.open("test_graph.json", std::ofstream::out | std::ofstream::trunc);
        out << "{\n\t\"vertices\": [\n\t\t";
        for (int i = 0; i < size_; i++) {
            out << "{\n\t\t\t\"id\": " << i << ",\n\t\t\t\"edge_ids\": [";
            int cnt(0);
            for (auto it = edges_.begin(); it != edges_.end(); it++) {
                if ((it->nodes[0] == i) || (it->nodes[1] == i)) {
                    cnt++;
                    if (cnt < vertices_[i].edges) {
                        out << it->id << ", ";
                    } else out << (*it).id;
                }
            }
            out << "]\n\t\t}";
            if (i != (size_-1)) out << ", ";
        }
        out << "\n\t],\n\t\"edges\": [\n\t\t";
        for(auto it = edges_.begin(); it != edges_.end(); it++) {
            out << "{\n\t\t\t\"id\": " << it->id << ",\n\t\t\t\"vertex_ids\": [" << it->nodes[0] << ", " << it->nodes[1] << "]\n\t\t}";
            if (it != edges_.end()-1) {
                out << ", ";
            }
        }
        out << "\n\t]\n}\n";
        out.close();
        return 0;
    }

};

int main() {
    vector<Edge> init_edges = { {0,1,0}, {0,2,1}, {0,3,2}, {1,4,3}, {1,5,4}, {1,6,5}, {2,7,6}, {2,8,7},
                            {3,9,8}, {4,10,9}, {5,10,10}, {6,10,11}, {7,11,12}, {8,11,13}, {9,12,14},
                            {10,13,15}, {11,13,16}, {12,13,17} };

    vector<Vertex> init_vertices = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13};

//    for (auto it = init_edges.begin(); it != init_edges.end(); it++) {
//        init_vertices[(*it).nodes[0]].edges++;
//        init_vertices[(*it).nodes[1]].edges++;
    for (const auto &it : init_edges) {
        init_vertices[it.nodes[0]].edges++;
        init_vertices[it.nodes[1]].edges++;
    }

    Graph A(init_edges, init_vertices);
    A.write_json();

    return 0;
}
