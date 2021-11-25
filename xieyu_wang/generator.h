#include<cstdlib>
#include <ctime>
#include <vector>
#include <string>
typedef struct connection{
    int fromVertex;
    int toVertex;
    std::string color;
}connection;
class Generator{
public :
    Generator(int maxDepth, int newVerticesNum);
    void verticesGenerator(int presentDepth, int fromVertexId );
    bool vertxGenerateProbabilityController(int depth);
private:
    int maxDepth_;
    int newVerticesNum_;
    int Depth_=0;
    std::vector<connection> connections_;
};