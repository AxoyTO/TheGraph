#include "graph.h"
    void Graph::addVertex() { vertices_.emplace_back(getNextVertexId()); }
    void Graph::addEdge(int fromVertexId, int toVertexId) {
        assert(hasVertex(fromVertexId) && "Vertex doesn't exist");
        assert(hasVertex(toVertexId) && "Vertex doesn't exist");
        assert(!isConnected(fromVertexId, toVertexId) &&
               "Vertices already connected");
        const auto& newEdge =
                edges_.emplace_back(getNextEdgeId(), fromVertexId, toVertexId);
        getVertex(fromVertexId).addEdgeId(newEdge.id);
        getVertex(toVertexId).addEdgeId(newEdge.id);
        getVertex(toVertexId).depth= getVertex(fromVertexId).depth+1;
    }
    std::string Graph::toString() const {
        // vertex
        std::string strGraph = "";
        strGraph += "{\n\t\"vertices\": [\n";
        for (const auto& vertex : vertices_) {
            strGraph += vertex.toString() + ",\n";
        }
        strGraph.pop_back();
        strGraph.pop_back();
        strGraph += "\n\t],\n";
        // edges
        strGraph += "\t\"edges\": [\n";
        for (const auto& edge : edges_) {
            strGraph += edge.toString() + ",\n";
        }
        strGraph.pop_back();
        strGraph.pop_back();
        strGraph += "\n\t]\n}\n";
        return strGraph;
    }
    bool Graph::hasVertex(int idFind) {
        for (const auto& vertex : vertices_) {
            if (vertex.id == idFind) {
                return true;
            }
        }
        return false;
    }
    bool Graph::isConnected(int fromVertexId, int toVertexId) {
        assert(hasVertex(fromVertexId) && "Vertex doesn't exist");
        assert(hasVertex(toVertexId) && "Vertex doesn't exist");
        for (const auto& fromVertexEdgeId : getVertex(fromVertexId).getEdgeIds()) {
            for (const auto& toVertexEdgeId : getVertex(toVertexId).getEdgeIds()) {
                if (fromVertexEdgeId == toVertexEdgeId) {
                    return true;
                }
            }
        }
        return false;
    }

    Vertex& Graph::getVertex(int id) {
        assert(hasVertex(id) && "Vertex doesn't exist");
        for (auto& vertex : vertices_) {
            if (vertex.id == id) {
                return vertex;
            }
        }
        throw std::runtime_error("Unreachable code");
    }

int Graph::getPresentVertexID() {
    return edgeIdCounter_;
}
