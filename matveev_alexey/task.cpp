#include <algorithm>
#include <array>
#include <cassert>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

using VertexId = int;
using EdgeId = int;

struct Vertex {
  const VertexId id;
  int depth = 0;

  explicit Vertex(const VertexId& _id) : id(_id) {}
};

struct Edge {
  enum class Colors { Grey, Green, Blue, Yellow, Red };

  const EdgeId id;
  const VertexId vertex_id1;
  const VertexId vertex_id2;
  const Colors color;
  explicit Edge(const EdgeId& _id,
                const VertexId& _vertex_id1,
                const VertexId& _vertex_id2,
                const Colors& _color)
      : id(_id),
        vertex_id1(_vertex_id1),
        vertex_id2(_vertex_id2),
        color(_color) {}
};

class Graph {
 public:
  bool hasVertex(const VertexId& vertex_id) const {
    for (const auto& vertex : vertexes_) {
      if (vertex.id == vertex_id) {
        return true;
      }
    }
    return false;
  }

  bool hasEdge(const EdgeId& edge_id) const {
    for (const auto& edge : edges_) {
      if (edge.id == edge_id) {
        return true;
      }
    }
    return false;
  }

  bool areConnected(const VertexId& vertex_id1,
                    const VertexId& vertex_id2) const {
    assert(hasVertex(vertex_id1) && "Vertex1 index is out of range");
    assert(hasVertex(vertex_id2) && "Vertex2 index is out of range");
    if (vertex_id1 == vertex_id2) {
      for (const auto& edge_id : connection_list_.at(vertex_id1)) {
        const auto& edge = getEdge(edge_id);
        if (edge.vertex_id1 == vertex_id1 && edge.vertex_id2 == vertex_id1) {
          return true;
        }
      }
    } else {
      for (const auto& edge_id : connection_list_.at(vertex_id1)) {
        const auto& edge = getEdge(edge_id);
        if (edge.vertex_id1 == vertex_id2 || edge.vertex_id2 == vertex_id2) {
          return true;
        }
      }
    }
    return false;
  }

  void addVertex() {
    const auto& new_vertex = vertexes_.emplace_back(getNewVertexId());
    connection_list_.insert({new_vertex.id, std::vector<EdgeId>()});
    if (layers_list_[0].size() == 0) {
      layers_list_[0].push_back(0);
    }
  }

  void addEdge(const VertexId& vertex_id1, const VertexId& vertex_id2) {
    assert(hasVertex(vertex_id1) && "Vertex1 index is out of range");
    assert(hasVertex(vertex_id2) && "Vertex2 index is out of range");
    assert(!areConnected(vertex_id1, vertex_id2) &&
           "These vertexes are already connected");
    Edge::Colors color;
    if (connection_list_[vertex_id2].size() == 0 ||
        connection_list_[vertex_id1].size() == 0) {
      color = Edge::Colors::Grey;
      if (vertex_id2 > vertex_id1) {
        for (auto& vertex : vertexes_) {
          if (vertex.id == vertex_id2) {
            vertex.depth = getVertex(vertex_id1).depth + 1;
            depth_ = std::max(depth_, vertex.depth);
            layers_list_[vertex.depth].push_back(vertex_id2);
            break;
          }
        }
      } else {
        for (auto& vertex : vertexes_) {
          if (vertex.id == vertex_id1) {
            vertex.depth = getVertex(vertex_id2).depth + 1;
            depth_ = std::max(depth_, vertex.depth);
            layers_list_[vertex.depth].push_back(vertex_id1);
            break;
          }
        }
      }
    } else if (vertex_id1 == vertex_id2) {
      color = Edge::Colors::Green;
    } else if (getVertex(vertex_id2).depth == getVertex(vertex_id1).depth) {
      color = Edge::Colors::Blue;
    } else if (std::abs(getVertex(vertex_id2).depth -
                        getVertex(vertex_id1).depth) == 1) {
      color = Edge::Colors::Yellow;
    } else {
      color = Edge::Colors::Red;
    }
    const auto& new_edge =
        edges_.emplace_back(getNewEdgeId(), vertex_id1, vertex_id2, color);
    connection_list_[vertex_id1].push_back(new_edge.id);
    if (vertex_id1 != vertex_id2) {
      connection_list_[vertex_id2].push_back(new_edge.id);
    }
  }

  const Edge& getEdge(const EdgeId& edge_id) const {
    assert(hasEdge(edge_id) && "Edge id is out of range.");
    for (const auto& edge : edges_) {
      if (edge.id == edge_id) {
        return edge;
      }
    }
    throw std::runtime_error("Cannot be reached.");
  }

  const std::vector<EdgeId> vertexConnections(const VertexId& id) const {
    assert(hasVertex(id) && "Vertex id is out of range");
    return connection_list_.at(id);
  }
  const std::vector<Vertex>& vertexes() const { return vertexes_; }
  const std::vector<Edge>& edges() const { return edges_; }
  const std::vector<VertexId>& ithLayer(int i) const {
    assert(i <= depth_ && "Graph is not that deep");
    return layers_list_.at(i);
  }
  const int vertexIdDepth(const VertexId& vertex_id) const {
    assert(hasVertex(vertex_id) && "Vertex id is out of range");
    for (const auto& vertex : vertexes_) {
      if (vertex.id == vertex_id) {
        return vertex.depth;
      }
    }
    throw std::runtime_error("Cannot be reached.");
  }

 private:
  std::vector<Vertex> vertexes_;
  std::vector<Edge> edges_;
  int depth_ = 0;
  int vertex_new_id_ = 0, edge_new_id_ = 0;
  std::unordered_map<VertexId, std::vector<EdgeId>> connection_list_;
  std::unordered_map<int, std::vector<VertexId>> layers_list_;
  VertexId getNewVertexId() { return vertex_new_id_++; }
  EdgeId getNewEdgeId() { return edge_new_id_++; }
  const Vertex& getVertex(const VertexId& vertex_id) const {
    assert(hasVertex(vertex_id) && "Vertex id is out of range.");
    for (const auto& vertex : vertexes_) {
      if (vertex.id == vertex_id) {
        return vertex;
      }
    }
    throw std::runtime_error("Cannot be reached.");
  }
};

class GraphGenerator {
 public:
  struct Params {
    explicit Params(int _depth = 0, int _new_vertexes_num = 0)
        : depth(_depth), new_vertexes_num(_new_vertexes_num) {}

    const int depth;
    const int new_vertexes_num;
  };

  explicit GraphGenerator(const Params& params = Params()) : params_(params) {}

  bool randValue(double probability) const {
    return probability > rand() % 100;
  }

  Graph generate() const {
    Graph graph;
    double step = 100.0 / params_.depth;
    graph.addVertex();
    int cur_depth = 0, vertexes_on_cur_depth, vertexes_on_prev_depth = 1;
    for (; cur_depth < params_.depth && vertexes_on_prev_depth > 0;
         cur_depth++) {
      vertexes_on_cur_depth = 0;
      for (const auto& vertex_id : graph.ithLayer(cur_depth)) {
        for (int j = 0; j < params_.new_vertexes_num; j++) {
          if (randValue(100.0 - step * cur_depth)) {
            graph.addVertex();
            graph.addEdge(vertex_id, graph.vertexes().back().id);
            vertexes_on_cur_depth++;
          }
        }
      }
      vertexes_on_prev_depth = vertexes_on_cur_depth;
    }
    if (cur_depth < params_.depth) {
      std::cout << "Depth of the graph is less than given. Depth is "
                << cur_depth << std::endl;
    }
    for (const auto& vertex : graph.vertexes()) {
      if (randValue(10)) {
        graph.addEdge(vertex.id, vertex.id);
      }
      if (randValue(25) && graph.hasVertex(vertex.id + 1) &&
          graph.vertexIdDepth(vertex.id + 1) == vertex.id) {
        graph.addEdge(vertex.id, vertex.id + 1);
      }
      if (vertex.depth < cur_depth &&
          randValue(100.0 * vertex.depth / (cur_depth - 1))) {
        std::vector<VertexId> next_layer;
        for (const auto& vertex_id : graph.ithLayer(vertex.depth + 1)) {
          if (!graph.areConnected(vertex.id, vertex_id)) {
            next_layer.push_back(vertex_id);
          }
        }
        graph.addEdge(vertex.id, next_layer[rand() % next_layer.size()]);
      }
      if (randValue(33) && vertex.depth < (cur_depth - 1)) {
        std::vector<VertexId> after_next_layer;
        for (const auto& vertex_id : graph.ithLayer(vertex.depth + 2)) {
          if (!graph.areConnected(vertex.id, vertex_id)) {
            after_next_layer.push_back(vertex_id);
          }
        }
        graph.addEdge(vertex.id,
                      after_next_layer[rand() % after_next_layer.size()]);
      }
    }
    return graph;
  }

 private:
  const Params params_ = Params();
};

class GraphPrinter {
 public:
  explicit GraphPrinter(const Graph& graph) : graph_(graph) {}
  std::string print() const {
    std::string graph_string;
    graph_string += "{\n \"vertices\": [\n  ";
    for (const auto& vertex : graph_.vertexes()) {
      graph_string += printVertex(vertex.id);
    }
    graph_string.pop_back();
    graph_string.pop_back();
    graph_string += "\n ],\n \"edges\": [\n  ";
    for (const auto& edge : graph_.edges()) {
      graph_string += printEdge(edge);
    }
    graph_string.pop_back();
    graph_string.pop_back();
    graph_string += "\n ]\n}\n";
    return graph_string;
  }
  std::string printVertex(const VertexId& id) const {
    std::string vertex_string =
        "{\n   \"id\": " + std::to_string(id) + ",\n   \"edge_ids\": [";
    for (const auto& edge_id : graph_.vertexConnections(id)) {
      vertex_string += std::to_string(edge_id) + ", ";
    }
    vertex_string.pop_back();
    vertex_string.pop_back();
    vertex_string +=
        "],\n   \"depth\": " + std::to_string(graph_.vertexIdDepth(id)) +
        "\n  }, ";
    return vertex_string;
  }
  std::string printEdge(const Edge& edge) const {
    std::string edge_string = "{\n   \"id\": " + std::to_string(edge.id);
    edge_string += ",\n   \"vertex_ids\": [";
    edge_string += std::to_string(edge.vertex_id1) + ", ";
    edge_string += std::to_string(edge.vertex_id2) + "],\n   ";
    edge_string += "\"color\": " + colorToString(edge) + "\n  }, ";
    return edge_string;
  }

 private:
  const Graph& graph_;
  std::string colorToString(Edge edge) const {
    switch (edge.color) {
      case Edge::Colors::Grey:
        return "\"gray\"";
      case Edge::Colors::Green:
        return "\"green\"";
      case Edge::Colors::Blue:
        return "\"blue\"";
      case Edge::Colors::Yellow:
        return "\"yellow\"";
      case Edge::Colors::Red:
        return "\"red\"";
    }
  }
};

constexpr int VERTEX_NUMBER = 14, EDGE_NUMBER = 18;

void write_to_file(const std::string& string, const std::string& file_name) {
  std::ofstream file(file_name);
  file << string;
  file.close();
}

int main() {
  int depth, new_vertexes_num;
  std::cout << "Enter depth:" << std::endl;
  std::cin >> depth;
  while (depth < 0) {
    std::cout << "Depth must be not negative. Enter depth again:" << std::endl;
    std::cin >> depth;
  }
  std::cout << "Enter number of new vertices:" << std::endl;
  std::cin >> new_vertexes_num;
  while (depth < 0) {
    std::cout << "Number of new vertices must be not negative. Enter number of "
                 "new vertexes again:"
              << std::endl;
    std::cin >> new_vertexes_num;
  }
  srand(time(NULL));
  GraphGenerator::Params params =
      GraphGenerator::Params(depth, new_vertexes_num);
  Graph graph = GraphGenerator(params).generate();
  const auto graph_printer = GraphPrinter(graph);
  const auto graph_json = graph_printer.print();
  std::cout << graph_json << std::endl;
  write_to_file(graph_json, "graph.json");
  return 0;
}
