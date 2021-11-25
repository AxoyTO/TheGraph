#include "generator.h"
Generator::Generator(int maxDepth, int newVerticesNum)
        : maxDepth_(maxDepth), newVerticesNum_(newVerticesNum) {
    srand((unsigned)time(NULL));
}

bool Generator::vertxGenerateProbabilityController(int depth) {
    float probability = (maxDepth_ - depth);
    probability = probability / maxDepth_ * 100;
    int randomNumber = rand() % 100 + 1;
    if (randomNumber > probability) {
        return false;
    }
    return true;
}

void Generator::verticesGenerator(int presentDepth = 1, int fromVertexId = 0) {
    if (presentDepth<maxDepth_){
        for(int i=0;i<newVerticesNum_;i++){
            if (vertxGenerateProbabilityController(presentDepth)){
            }
        }
    }
}
