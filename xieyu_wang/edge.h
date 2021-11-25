#ifndef TASK_04_EDGE_H
#define TASK_04_EDGE_H

#include <string>

class Edge {
public:
    const int id = 0;
    const int fromVertexId = 0;
    const int toVertexId = 0;
    const std::string color;
    Edge(int id, int fromVertexId, int toVertexId,const std::string color);
    std::string toString() const;
};
#endif //TASK_04_EDGE_H
