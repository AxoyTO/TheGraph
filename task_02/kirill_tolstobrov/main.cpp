#include <iostream>
#include <map>
#include <cstdlib>
#include <ctime>
#include <iterator>
#include <vector>
#include <fstream>
#include <utility>

class Edge;

class Vertex {

    int id;
    //access to the edge by id of connected vertex
    std::map<int, Edge *> edges;
    std::map<int, Vertex *> vertices;
    std::vector<int> edges_ids;

public:
    Vertex(int x = 0): id(x) {}
    int get_id() { return id; }

    void bindWithVertex(Vertex *vx, Edge *ed, int new_edge_id) {
        vertices[vx->id] = vx;
        edges[vx->id] = ed;
        edges_ids.push_back(new_edge_id);
    }

    operator std::string() {
        std::string res = "";
        res += "    {\n      \"id\": " + std::to_string(id);
        res += ",\n      \"edge_ids\": ";
        for (auto it = edges_ids.begin(); it != edges_ids.end(); it++) {
            if(it == edges_ids.begin()) {
                res += "[";
            } else {
                res += ", ";
            }
            res += std::to_string(*it);
        }
        res += "]\n    }";
        return res;
    } 

};

class Edge{
    int id;
    int is_binded;
    std::map<int, Vertex *> vertices;

public:
    Edge(int x = 0): id(x), is_binded(0) {};
    int get_id() { return id; }

    void bindWithVertices(Vertex *vx1, Vertex *vx2) {
        if (is_binded) {
            return;
        }
        is_binded = 1;
        vertices[vx1->get_id()] = vx1;
        vertices[vx2->get_id()] = vx2;
    }

    operator std::string() {
        std::string res = "";
        res += "    {\n      \"id\": " + std::to_string(id);
        res += ",\n      \"edge_ids\": ";
        for (auto it = vertices.begin(); it != vertices.end(); it++) {
            if(it == vertices.begin()) {
                res += "[";
            } else {
                res += ", ";
            }
            res += std::to_string(it->second->get_id());
        }
        res += "]\n    }";
        return res;
    }

};

class Graph{
    std::map<int, Edge> edges;
    std::map<int, Vertex> vertices;
    int vertices_amount;
    int edges_amount;

public:
    Graph(std::string mode = "empty", int max_vertices = 10, int max_edges = 15) {
        vertices_amount = 0;
        edges_amount = 0;
        if(mode == "empty") {
            return;
        } else if (mode != "random") {
            std::cout << "Wrong mode. Empty graph was created" << std::endl;
            return;
        }

        enum {
            PROBABILITY_COEF = 100
        };

        srand(std::time(NULL));

        for(int i = 0; i < max_vertices; i++) {
            double new_vertex_prob = PROBABILITY_COEF - (double)PROBABILITY_COEF * vertices_amount / max_vertices;
            if ( ((double)std::rand() / RAND_MAX) * PROBABILITY_COEF < new_vertex_prob) {
                this->AddNewVertex();
                if (vertices_amount == 1) {
                    continue;
                }

                int vertex_to_bind = std::rand() % (vertices_amount-1);
                this->BindVertices(vertices_amount - 1, vertex_to_bind);
            }
        }

        for(int i = edges_amount; i < max_edges; i++) {
            double new_edge_prob = PROBABILITY_COEF - (double)PROBABILITY_COEF * edges_amount / max_edges;
            if ( ((double)std::rand() / RAND_MAX) * PROBABILITY_COEF < new_edge_prob) {
                int vertex_to_bind1 = std::rand() % vertices_amount;
                int vertex_to_bind2 = std::rand() % vertices_amount;
                this->BindVertices(vertex_to_bind1, vertex_to_bind2);
            }
        }
    }

    Graph(int vert_number, std::vector<std::pair<int, int>> connections) {
        vertices_amount = 0;
        edges_amount = 0;
        for(int i = 0; i < vert_number; i++) {
            this->AddNewVertex();
        }
        for(int i = 0; i < connections.size(); i++) {
            this->BindVertices(connections[i].first, connections[i].second);
        }
    }

    int get_vertices_amount() { return vertices_amount; }
    int get_edges_amount() { return edges_amount; }

    void AddNewVertex(){
        vertices[vertices_amount] = Vertex(vertices_amount);
        vertices_amount++;
    }

    void BindVertices(int id1, int id2) {
        edges[edges_amount] = Edge(edges_amount);
        edges_amount++;
        edges[edges_amount - 1].bindWithVertices(&vertices[id1], &vertices[id2]);
        vertices[id1].bindWithVertex(&vertices[id2], &edges[edges_amount - 1], edges_amount - 1);
        vertices[id2].bindWithVertex(&vertices[id1], &edges[edges_amount - 1], edges_amount - 1);
    }

    operator std::string() {
        std::string result = "{\n  \"vertices\": [\n";
        for(int i = 0; i < vertices_amount; i++) {
            result += (std::string)vertices[i];
            if(i != vertices_amount - 1)
                result += ",\n";
        }
        result += "\n  ],\n  \"edges\": [\n";
        for(int i = 0; i < edges_amount; i++) {
            result += (std::string)edges[i];
            if(i != edges_amount - 1)
                result += ",\n";
        }
        result += "\n  ]\n}";
        return result;
    }

};



int main() {

    Graph g("random");

    std::ofstream file;
    file.open("graph.json");

    if(file.is_open()) {
        file << (std::string)g << std::endl;
    }

    return 0;
}
