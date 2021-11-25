#include "generator.h"
Generator::Generator(int maxDepth, int newVerticesNum)
    : maxDepth_(maxDepth), newVerticesNum_(newVerticesNum) {
  if (maxDepth < 0) {
    while (maxDepth_ < 0) {
      std::cout << "Error Max Depth must be  above or equal 0: ";
      std::cin >> maxDepth_;
    }
  }
  if (newVerticesNum < 0) {
    while (newVerticesNum_ < 0) {
      std::cout << "Error new Vertices number must be above or equal 0: ";
      std::cin >> newVerticesNum_;
    }
  }
}

bool Generator::vertxGenerateProbabilityController(int depth) {
  float prob = (maxDepth_ - depth);
  prob = prob / maxDepth_;
  std::bernoulli_distribution probability(prob);
  return probability(generator_);
}

Graph Generator::generateGrey(int presentDepth = 1, int fromVertexId = 0) {
  Graph graph;
  graph.addVertex();
  std::vector<int> verticesPresentLevel;
  for (int depth = 0; depth < maxDepth_; depth++) {
    const auto& vertexIds = graph.getVertexIdsAtDepth(depth);
    bool isNewVertexGenerated = false;
    for (const auto& vertexId : vertexIds) {
      for (int i = 0; i < newVerticesNum_; ++i) {
        if (vertxGenerateProbabilityController(presentDepth)) {
          const auto& newVertex = graph.addVertex();
          graph.addEdge(vertexId, newVertex.id, Edge::Color::Gray);
          isNewVertexGenerated = true;
        }
      }
    }
    if (!isNewVertexGenerated) {
      break;
    }
  }
  return graph;
}
void Generator::generateGreen(Graph& graph) {
  int verticesSum = graph.getPresentVertexId();
  for (int i = 0; i < verticesSum; ++i) {
    if (specialEdgeGenerateProbabilityController(Edge::Color::Green)) {
      graph.addEdge(i, i, Edge::Color::Green);
    }
  }
}
void Generator::generateRed(Graph& graph) {
  int nexDepth;
  for (int depth = 0; depth < maxDepth_; depth++) {
    nexDepth = depth + 2;
    const auto presentDepth = graph.getVertexIdsAtDepth(depth);
    const auto destinationDepth = graph.getVertexIdsAtDepth(nexDepth);
    for (const auto& fromVertex : presentDepth) {
      for (const auto& toVertex : destinationDepth) {
        if (specialEdgeGenerateProbabilityController(Edge::Color::Red)) {
          graph.addEdge(fromVertex, toVertex, Edge::Color::Red);
        }
      }
    }
  }
}
void Generator::generateBlue(Graph& graph) {
  for (int depth = 1; depth < maxDepth_; depth++) {
    const auto presentDepth = graph.getVertexIdsAtDepth(depth);
    for (auto vertex = presentDepth.begin(); vertex < presentDepth.end();
         vertex++) {
      if (*vertex != presentDepth.back() &&
          specialEdgeGenerateProbabilityController(Edge::Color::Blue)) {
        graph.addEdge(*vertex, *(vertex + 1), Edge::Color::Blue);
      }
    }
  }
}
void Generator::generateYellow(Graph& graph) {
  for (int depth = 0; depth < maxDepth_; depth++) {
    const auto presentLevel = graph.getVertexIdsAtDepth(depth);
    const auto destinationLevel = graph.getVertexIdsAtDepth(depth + 1);
    for (const auto& fromVertex : presentLevel) {
      for (const auto& toVertex : destinationLevel) {
        if (specialEdgeGenerateProbabilityController(Edge::Color::Yellow,
                                                     depth) &&
            !graph.isConnected(fromVertex, toVertex)) {
          graph.addEdge(fromVertex, toVertex, Edge::Color::Yellow);
        }
      }
    }
  }
}

bool Generator::specialEdgeGenerateProbabilityController(Edge::Color color,
                                                         int depth) {
  if (color == Edge::Color::Red) {
    std::bernoulli_distribution probability(RED_EDGE_PROBABILITY);
    return probability(generator_);
  } else if (color == Edge::Color::Blue) {
    std::bernoulli_distribution probability(BLUE_EDGE_PROBABILITY);
    return probability(generator_);
  } else if (color == Edge::Color::Green) {
    std::bernoulli_distribution probability(GREEN_EDGE_PROBABILITY);
    return probability(generator_);
  } else if (color == Edge::Color::Yellow) {
    if (vertxGenerateProbabilityController(depth)) {
      return false;
    }
  }
  return true;
}

Graph Generator::generate() {
  std::cout << "Generator.................done" << std::endl;
  auto graph = generateGrey();
  std::cout << "GrayEdgeGenerate..........done" << std::endl;
  generateGreen(graph);
  std::cout << "GreenEdgeGenerate.........done" << std::endl;
  generateBlue(graph);
  std::cout << "BlueEdgeGenerate..........done" << std::endl;
  generateRed(graph);
  std::cout << "redEdgeGenerate...........done" << std::endl;
  generateYellow(graph);
  std::cout << "yellowEdgeGenerate........done" << std::endl;
  return graph;
}
