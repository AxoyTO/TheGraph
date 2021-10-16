#include <array>
#include <fstream>
#include <string>
#include <vector>
class Vertex {
public:
    const int id = 0;
    explicit Vertex( int _id) : id(_id) {}
    void addEdgeId(int eId) { edgeIds.push_back(eId); }
    std::string toString() {
        std::string str = "";
        str += "\t\t{\"id\":" + std::to_string(id) + ",";
        str += "\"edge_ids\": [";
        for (const auto edgeId : edgeIds) {
            str += std::to_string(edgeId) + ",";
        }
        str.pop_back();
        str += "]}";
        return str;
    }

private:
    std::vector<int> edgeIds;
};

class Edge {
public:
    const int id = 0;
    const int fromVertexId = 0;
    const int toVertexId = 0;
    Edge(const int id, const int fromVertexId, const int toVertexId)
            : id(id), fromVertexId(fromVertexId), toVertexId(toVertexId) {}
    std::string toString() {
        std::string strEdge = "";
        strEdge += "\t\t{\"id\": " + std::to_string(id) + ",";
        strEdge += "\"vertex_ids\": [" + std::to_string(fromVertexId) + "," +
                   std::to_string(toVertexId) + "]}";
        return strEdge;
    }
};
class Graph {
public:
    // toSting
    void addVertx(int from, int edge, int to) {
        if (!isIdInVertex(from)) {
            vertexs.emplace_back(from);
        }
        for (auto &vertexId : vertexs) {
            if (vertexId.id == from) {
                vertexId.addEdgeId(edge);
            }
        }
        if (!isIdInVertex(to)) {
            vertexs.emplace_back(to);
        }
        for (auto &vertexId : vertexs) {
            if (vertexId.id == to) {
                vertexId.addEdgeId(edge);
            }
        }
    }
    void addEdge(int from, int _edge, int to) {
        edges.emplace_back(_edge, from, to);
    }
    std::string toString() {
        // vertex
        std::string strGraph = "";
        strGraph += "{\n\t\"vertices\": [\n";
        for (auto vertexId : vertexs) {
            strGraph += vertexId.toString() + ",\n";
        }
        strGraph.pop_back();
        strGraph.pop_back();
        strGraph += "\n\t],\n";
        // edges
        strGraph += "\t\"edges\": [\n";
        for (auto edgeId : edges) {
            strGraph += edgeId.toString() + ",\n";
        }
        strGraph.pop_back();
        strGraph.pop_back();
        strGraph += "\n\t]\n}\n";
        return strGraph;
    }

private:
    bool isIdInVertex(int _id) {
        for (const auto vertexId : vertexs) {
            if (vertexId.id == _id) {
                return true;
            }
        }
        return false;
    }
    std::vector<Vertex> vertexs;
    std::vector<Edge> edges;
};
int main() {
    const std::array<std::array<int, 3>, 18> netWork = {{{0, 0, 1},
                                                             {0, 1, 2},
                                                             {0, 2, 3},
                                                             {1, 3, 4},
                                                             {1, 4, 5},
                                                             {1, 5, 6},
                                                             {2, 6, 7},
                                                             {2, 7, 8},
                                                             {3, 8, 9},
                                                             {4, 9, 10},
                                                             {5, 10, 10},
                                                             {6, 11, 10},
                                                             {7, 12, 11},
                                                             {8, 13, 11},
                                                             {9, 14, 12},
                                                             {10, 15, 13},
                                                             {11, 16, 13},
                                                             {12, 17, 13}}};
    Graph graph;
    for (const auto netId : netWork) {
        graph.addVertx(netId.at(0), netId.at(1), netId.at(2));
        graph.addEdge(netId.at(0), netId.at(1), netId.at(2));
    }
    std::ofstream writePT;
    writePT.open("Graphic.json", std::ios::out);
    writePT << graph.toString() << std::endl;
    writePT.close();
    return 0;
}