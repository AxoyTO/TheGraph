#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <utility>
#include <vector>

class Graph {
   public:
    struct Edge {
        int id;
        std::pair<int, int> vertices;
    };

    struct Vertex {
        int id;
        std::set<int> edges;
    };

    Graph(std::vector<Edge> inpEdges) {
        printf("Graph constructor by edges\n");
        for (auto edge : inpEdges) {
            auto e = edges.find(edge.id);
            if (e == edges.end()) {
                // add edge
                edges.emplace(edge.id, edge);
            } else {
                // edit edge
                e->second.vertices = edge.vertices;
            }
        }
        for (auto edge : edges) {
            auto vs = edge.second.vertices;
            int vsi[] = {vs.first, vs.second};
            for (int vid : vsi) {
                auto v = vertices.find(vid);
                if (v == vertices.end()) {
                    // add vertex
                    Vertex newVertex = {vid, {edge.first}};
                    vertices.emplace(vid, newVertex);
                } else {
                    // edit vertex
                    v->second.edges.insert(edge.first);
                }
            }
        }
    }

    void show() {
        printf("  * Vertices:\n");
        for (auto vertex_pair : vertices) {
            int id = vertex_pair.first;
            printf("[%2d]  <", id);
            for (int edgeId : vertex_pair.second.edges) {
                printf("%d, ", edgeId);
            }
            printf(">\n");
        }
        printf("  * Edges:\n");
        for (auto edge_pair : edges) {
            int id = edge_pair.first;
            std::pair<int, int> v = edge_pair.second.vertices;
            printf("[%2d]  <%d, %d>\n", id, v.first, v.second);
        }
    }

    void printJSON(char const* filename) {
        std::ofstream json(filename);
        if (json.is_open()) {
            printf("  * Print JSON to %s\n", filename);
            json << "{\n  \" vertices\": [\n    ";

            for (auto vertex_pair : vertices) {
                auto v = vertex_pair.second;
                json << "{\n      \"id\": " << v.id << ",\n";
                json << "      \"edge_ids\": [";

                /*auto it = edges.end();
                it--;
                int lastId = it->first;
                for (int edgeId : v.edges) {
                    json << edgeId;
                    if (edgeId != lastId) {
                        json << ",";
                    }
                    json << " ";
                }*/
                for (auto peid = v.edges.begin();;) {
                    json << *peid;
                    if (++peid != v.edges.end()) {
                        json << ", ";
                    } else {
                        break;
                    }
                }
                json << "]\n    }, ";
            }
            json << "\n  ],\n  \"edges\": [\n    ";
            /*printf("  * Edges:\n");
            for (auto edge_pair : edges) {
                int id = edge_pair.first;
                std::pair<int, int> v = edge_pair.second.vertices;
                printf("[%2d]  <%d, %d>\n", id, v.first, v.second);
            }*/
            json << "\n\t]\n}";
            json.close();
        } else {
            std::cout << "Unable to open file";
        }
    }

   private:
    std::map<int, Edge> edges;
    std::map<int, Vertex> vertices;
};

int main(void) {
    printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");

    std::vector<Graph::Edge> edges = {
        {0, {0, 1}},    {1, {0, 2}},   {2, {0, 3}},   {3, {1, 4}},
        {4, {1, 5}},    {5, {1, 6}},   {6, {2, 7}},   {7, {2, 8}},
        {8, {3, 9}},    {9, {4, 10}},  {10, {5, 10}}, {11, {6, 10}},
        {12, {7, 11}},  {13, {8, 11}}, {14, {9, 12}}, {15, {10, 13}},
        {16, {11, 13}}, {17, {12, 13}}};

    Graph g(edges);
    g.printJSON("graph.json");
    return 0;
}

int main_() {
    std::ofstream myfile("example.txt");
    if (myfile.is_open()) {
        myfile << "This is a line.\n";
        myfile << "This is another line.\n";
        myfile.close();
    } else {
        std::cout << "Unable to open file";
    }
    return 0;
}