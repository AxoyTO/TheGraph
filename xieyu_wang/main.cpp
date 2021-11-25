#include <array>
#include <fstream>
#include <string>
#include <vector>
#include "graph.h"
int main() {
    const int numberOfVertices = 18;
    const int verticesCount = 14;
    const std::array<std::pair<int, int>, numberOfVertices> connections = {
            {{0, 1},
             {0, 2},
             {0, 3},
             {1, 4},
             {1, 5},
             {1, 6},
             {2, 7},
             {2, 8},
             {3, 9},
             {4, 10},
             {5, 10},
             {6, 10},
             {7, 11},
             {8, 11},
             {9, 12},
             {10, 13},
             {11, 13},
             {12, 13}}};
    Graph graph;
    for (int i = 0; i < verticesCount; i++) {
        graph.addVertex();
    }
    for (const auto& connection : connections) {
        graph.addEdge(connection.first, connection.second);
    }
    std::ofstream writePT;
    writePT.open("Graph.json", std::ios::out);
    writePT << graph.toString() << std::endl;
    writePT.close();
    return 0;
}