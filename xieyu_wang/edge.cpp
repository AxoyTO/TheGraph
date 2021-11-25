#include "edge.h"
#include <string>
Edge::Edge(int id, int fromVertexId, int toVertexId,const std::string color)
        : id(id), fromVertexId(fromVertexId), toVertexId(toVertexId),color(color) {}
std::string Edge::toString() const {
    std::string strEdge = "";
    strEdge += "\t\t{\"id\": " + std::to_string(id) + ",";
    strEdge += "\"vertex_ids\": [" + std::to_string(fromVertexId) + "," +
               std::to_string(toVertexId) + "], \"color\":\"" + color + "\" }";
    return strEdge;
}