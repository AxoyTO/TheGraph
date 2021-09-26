#include <fstream>
#include <iostream>
#include <map>
#include <set>
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
        printf(" Graph:\n");
        printf("  * Vertices:\n");
        for (auto vertex_pair : vertices) {
            auto v = vertex_pair.second;
            printf("[%2d]  <", v.id);
            for (int edgeId : v.edges) {
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
            json << "{\n  \" vertices\": [\n    ";
            for (auto pVertexPair = vertices.begin();;) {
                auto v = pVertexPair->second;
                json << "{\n      \"id\": " << v.id;
                json << ",\n      \"edge_ids\": [";

                for (auto pEdgeId = v.edges.begin();;) {
                    json << *pEdgeId;
                    if (++pEdgeId != v.edges.end()) {
                        json << ", ";
                    } else {
                        break;
                    }
                }
                json << "]\n    }";
                if (++pVertexPair != vertices.end()) {
                    json << ", ";
                } else {
                    break;
                }
            }

            // -------------------------------

            json << "\n  ],\n  \"edges\": [\n    ";
            for (auto pEdgePair = edges.begin();;) {
                int edgeId = pEdgePair->first;
                auto vs = pEdgePair->second.vertices;
                json << "{\n      \"id\": " << edgeId;
                json << ",\n      \"vertex_ids\": [";
                json << vs.first << ", " << vs.second;
                json << "]\n    }";

                if (++pEdgePair != edges.end()) {
                    json << ", ";
                } else {
                    break;
                }
            }

            json << "\n\t]\n}\n";
            json.close();
            printf("Graph is written to %s\n", filename);
        } else {
            printf("Unable to open file\n");
        }
    }

   private:
    std::map<int, Edge> edges;
    std::map<int, Vertex> vertices;
};
