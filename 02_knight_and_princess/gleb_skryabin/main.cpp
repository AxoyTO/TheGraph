#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <utility>
#include <vector>

//------

class Graph {
   public:
    struct Edge {
        int id;
        std::pair<int, int> vertices;  // = {-1, -1};
    };

    // template <typename T>
    /*std::ostream& operator<<(std::ostream& o, const Edge<T>& a) {
        o << "id: " << p.id << "\tfirst: " << p.vertices.first << std::endl;
        return o;
    }*/

    struct Vertex {
        int id;
        std::set<int> edges;  // = {};
    };

    Graph(std::vector<Edge> inpEdges) {
        printf("Graph constructor by edges\n");
        for (auto edge : inpEdges) {
            /*auto result = edges.emplace(edge.id, edge);
            if (!result.second) {
                edges[edge.id] = edge;
            }*/
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
        printf(" * Edges:\n");
        for (auto edge_pair : edges) {
            int id = edge_pair.first;
            std::pair<int, int> v = edge_pair.second.vertices;
            printf("[%2d]  <%d, %d>\n", id, v.first, v.second);
        }
        printf(" * Vertices:\n");
        for (auto vertex_pair : vertices) {
            int id = vertex_pair.first;
            printf("[%2d]  <", id);
            for (int edgeId : vertex_pair.second.edges) {
                printf("%d, ", edgeId);
            }
            printf(">\n");
        }
    }

   private:
    std::map<int, Edge> edges;
    std::map<int, Vertex> vertices;

    /*template <typename T>
    void addToMap(std::map<int, T> map, T el) {
        // weg
        // wef
    }*/
    // void dbg(char* s) { printf("  * %s\n", &s); }
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
    g.show();
    return 0;
}
