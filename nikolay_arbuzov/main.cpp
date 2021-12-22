#include <algorithm>
#include <array>
#include <cassert>
#include <fstream>
#include <iostream>
#include <random>
#include <set>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

using VertexId = int;
using EdgeId = int;

constexpr int kVerticesCount = 14;

class Graph {
 public:
  using Depth = int;

  struct Vertex {
    explicit Vertex(const VertexId& _id) : id(_id){};
    VertexId id = 0;
    Graph::Depth depth = 0;
  };

  struct Edge {
    enum class Color { Grey, Green, Yellow, Red };
    explicit Edge(const EdgeId& _id,
                  const Color& _color,
                  const VertexId& _from_vertex_id,
                  const VertexId& _to_vertex_id)
        : id(_id),
          color(_color),
          from_vertex_id(_from_vertex_id),
          to_vertex_id(_to_vertex_id) {}
    const Color color = Color::Grey;
    EdgeId id = 0;
    const VertexId from_vertex_id = 0;
    const VertexId to_vertex_id = 0;
  };

  Vertex& add_vertex() {
    VertexId new_vertex_id = get_new_vertex_id();
    adjacency_list_[new_vertex_id] = std::set<EdgeId>();
    return vertices_.emplace_back(new_vertex_id);
  };

  bool has_vertex(const VertexId& vertex_id) const {
    for (auto vertex : vertices_) {
      if (vertex.id == vertex_id) {
        return true;
      }
    }
    return false;
  };

  const Vertex& get_vertex(const VertexId& vertex_id) const {
    for (const auto& vertex : vertices_) {
      if (vertex.id == vertex_id) {
        return vertex;
      }
    }
    throw std::runtime_error("No such vertex");
  }
  const Edge& get_edge(const EdgeId& edge_id) const {
    for (const auto& edge : edges_) {
      if (edge.id == edge_id) {
        return edge;
      }
    }
    throw std::runtime_error("No such edge");
  }

  const Depth& get_vertex_depth(const VertexId& vertex_id) const {
    const Vertex& vertex = get_vertex(vertex_id);
    return vertex.depth;
  }

  void set_vertex_depth(const VertexId& vertex_id, const Depth& depth) {
    for (auto& vertex : vertices_) {
      if (vertex.id == vertex_id) {
        vertex.depth = depth;
        break;
      }
    }
  }

  bool is_connected(const VertexId& from_vertex_id,
                    const VertexId& to_vertex_id) const {
    const auto& from_edge_set = adjacency_list_.at(from_vertex_id);
    const auto& to_edge_set = adjacency_list_.at(to_vertex_id);
    std::set<EdgeId> intersection;
    std::set_intersection(from_edge_set.begin(), from_edge_set.end(),
                          to_edge_set.begin(), to_edge_set.end(),
                          std::inserter(intersection, intersection.begin()));
    if (intersection.size()) {
      return true;
    }
    return false;
  }

  const Edge::Color color_determining(const VertexId& from_vertex_id,
                                      const VertexId& to_vertex_id) const {
    const auto from_vertex_depth = get_vertex_depth(from_vertex_id);
    const auto to_vertex_depth = get_vertex_depth(to_vertex_id);
    if (from_vertex_id == to_vertex_id) {
      return Edge::Color::Green;
    }
    if (adjacency_list_.at(to_vertex_id).size() == 0) {
      return Edge::Color::Grey;
    }
    if (to_vertex_depth - from_vertex_depth == 1 &&
        !is_connected(from_vertex_id, to_vertex_id)) {
      return Edge::Color::Yellow;
    }
    if (to_vertex_depth - from_vertex_depth == 2) {
      return Edge::Color::Red;
    }
    throw std::runtime_error("Failed to determine color");
  }

  void set_graph_depth(const Depth& depth) { depth_ = depth; }

  Edge& add_edge(const VertexId& from_vertex_id, const VertexId& to_vertex_id) {
    assert(has_vertex(from_vertex_id));
    assert(has_vertex(to_vertex_id));
    std::cout << "Try to determine color of edge from: " << from_vertex_id
              << ", to: " << to_vertex_id << std::endl;
    const auto& new_edge_color =
        color_determining(from_vertex_id, to_vertex_id);

    std::cout << "It's ";

    switch (new_edge_color) {
      case Graph::Edge::Color::Grey:
        std::cout << "Grey" << std::endl;
        break;
      case Graph::Edge::Color::Green:
        std::cout << "Green" << std::endl;
        break;
      case Graph::Edge::Color::Yellow:
        std::cout << "Yellow" << std::endl;
        break;
      case Graph::Edge::Color::Red:
        std::cout << "Red" << std::endl;
        break;
      default:
        throw std::runtime_error("No such color");
        break;
    }

    EdgeId new_edge_id = get_new_edge_id();

    adjacency_list_[from_vertex_id].insert(new_edge_id);
    adjacency_list_[to_vertex_id].insert(new_edge_id);

    auto& new_edge = edges_.emplace_back(new_edge_id, new_edge_color,
                                         from_vertex_id, to_vertex_id);

    if (new_edge_color == Edge::Color::Grey) {
      const auto from_vertex_depth = get_vertex_depth(from_vertex_id);
      set_graph_depth(from_vertex_depth + 1);
      set_vertex_depth(to_vertex_id, from_vertex_depth + 1);
    }

    return new_edge;
  };

  bool has_edge(const EdgeId& edge_id) const {
    for (auto edge : edges_) {
      if (edge.id == edge_id) {
        return true;
      }
    }
    return false;
  };

  std::vector<Vertex> get_vertices_on_depth(const Depth& depth) const {
    std::vector<Vertex> vertices_on_depth;
    for (const auto& vertex : vertices_) {
      if (vertex.depth == depth) {
        vertices_on_depth.push_back(vertex);
      }
    }
    return vertices_on_depth;
  }

  const std::vector<Vertex>& vertices() const { return vertices_; }
  const std::vector<Edge>& edges() const { return edges_; }
  const Depth& depth() const { return depth_; }

  const std::set<EdgeId>& connected_edge_ids(const VertexId& id) const {
    return adjacency_list_.at(id);
  }

 private:
  std::vector<Edge> edges_;
  std::vector<Vertex> vertices_;
  std::unordered_map<VertexId, std::set<EdgeId>> adjacency_list_;

  EdgeId edges_id_counter_ = 0;
  VertexId vertex_id_counter_ = 0;
  Depth depth_ = 0;

  VertexId get_new_vertex_id() { return vertex_id_counter_++; }
  EdgeId get_new_edge_id() { return edges_id_counter_++; }
};

namespace graph_printing {
std::string print_edge_color(const Graph::Edge::Color& color) {
  switch (color) {
    case Graph::Edge::Color::Grey:
      return "Grey";
      break;
    case Graph::Edge::Color::Green:
      return "Green";
      break;
    case Graph::Edge::Color::Yellow:
      return "Yellow";
      break;
    case Graph::Edge::Color::Red:
      return "Red";
      break;
    default:
      throw std::runtime_error("No such color");
      break;
  }
}
std::string print_vertex(const Graph& graph, const Graph::Vertex& vertex) {
  std::stringstream result_stream;
  result_stream << "\t\t{" << std::endl;
  result_stream << "\t\t\t\"id\": " << vertex.id << "," << std::endl;
  result_stream << "\t\t\t\"edge_ids\": [";

  const auto& edge_ids = graph.connected_edge_ids(vertex.id);

  for (const auto& edge_id : edge_ids) {
    result_stream << edge_id;
    if (edge_id != *(edge_ids.rbegin())) {
      result_stream << ", ";
    }
  }

  result_stream << "]," << std::endl;
  result_stream << "\t\t\t\"depth\": " << vertex.depth << std::endl;
  result_stream << "\t\t}";

  return result_stream.str();
}

std::string print_edge(const Graph::Edge& edge) {
  std::stringstream result_stream;
  result_stream << "\t\t{" << std::endl;
  result_stream << "\t\t\t\"id\": " << edge.id << "," << std::endl;
  result_stream << "\t\t\t\"vertex_ids\": [";
  result_stream << edge.from_vertex_id << ", " << edge.to_vertex_id;
  result_stream << "]," << std::endl;
  result_stream << "\t\t\t\"color\": \"" << print_edge_color(edge.color) << "\""
                << std::endl;
  result_stream << "\t\t}";

  return result_stream.str();
}

std::string print_graph(const Graph& graph) {
  std::stringstream result_stream;
  std::cout << "Printing graph to string" << std::endl;

  result_stream << "{" << std::endl;
  result_stream << "\t\"depth\": " << graph.depth() << "," << std::endl;

  std::cout << "\tPrinting vertices to string" << std::endl;

  result_stream << "\t\"vertices\": [" << std::endl;

  const auto& vertices = graph.vertices();
  for (const auto& vertex : vertices) {
    result_stream << print_vertex(graph, vertex);
    if (vertex.id != vertices.back().id) {
      result_stream << ",";
    }
    result_stream << std::endl;
  }

  result_stream << std::endl
                << "\t]"
                << "," << std::endl;

  const auto& edges = graph.edges();
  std::cout << "\tPrinting edges to string" << std::endl;
  result_stream << "\t\"edges\": [" << std::endl;
  for (const auto& edge : edges) {
    result_stream << print_edge(edge);
    if (edge.id != edges.back().id) {
      result_stream << ",";
    }
    result_stream << std::endl;
  }
  result_stream << "\t]" << std::endl << "}";
  return result_stream.str();
}

}  // namespace graph_printing
void write_to_file(const std::string& graph_json,
                   const std::string& file_path) {
  std::ofstream out_file;
  out_file.open(file_path);
  out_file << graph_json;
  out_file.close();
}
class GraphGenerator {
 public:
  struct Params {
   public:
    explicit Params(Graph::Depth depth = 0, int new_vertices_count = 0)
        : depth_(depth), new_vertices_count_(new_vertices_count) {}

    Graph::Depth depth() const { return depth_; }
    int new_vertices_count() const { return new_vertices_count_; }

   private:
    Graph::Depth depth_ = 0;
    int new_vertices_count_ = 0;
  };

  explicit GraphGenerator(const Params& params = Params()) : params_(params) {}

  void generate_grey_edges(Graph& graph) {
    Graph::Depth depth = params_.depth();
    int new_vertices_count = params_.new_vertices_count();
    std::random_device random_device;
    std::mt19937 generate(random_device());
    for (Graph::Depth depth_i = 0; depth_i < depth; ++depth_i) {
      std::bernoulli_distribution distribution(float(depth - depth_i) / depth);
      for (const auto& vertex : graph.get_vertices_on_depth(depth_i)) {
        for (int new_vertices_i = 0; new_vertices_i < new_vertices_count;
             ++new_vertices_i) {
          if (distribution(generate)) {
            const auto& new_vertex = graph.add_vertex();
            graph.add_edge(vertex.id, new_vertex.id);
            // std::cout << "New vertex:\n\tid = " << new_vertex.id << ",\tdepth
            // = "<< new_vertex.depth << std::endl;
          }
        }
      }
    }
  }

  void generate_green_edges(Graph& graph) {
    const auto& vertices = graph.vertices();
    std::random_device random_device;
    std::mt19937 generate(random_device());
    std::bernoulli_distribution distribution(0.1);
    for (const auto& vertex : vertices) {
      if (distribution(generate))
        graph.add_edge(vertex.id, vertex.id);
    }
  }

  void generate_yellow_edges(Graph& graph) {
    Graph::Depth depth = params_.depth();
    std::random_device random_device;
    std::mt19937 generate(random_device());
    for (Graph::Depth depth_i = 0; depth_i < depth - 1; ++depth_i) {
      std::bernoulli_distribution distribution(
          float(1) - (float(depth - 1 - depth_i) / (depth - 1)));
      for (const auto& vertex : graph.get_vertices_on_depth(depth_i)) {
        const auto& to_vertices = graph.get_vertices_on_depth(depth_i + 1);
        auto to_vertices_no_neighbors = std::vector<Graph::Vertex>();

        for (const auto& not_neighbor_vertex : to_vertices) {
          if (!graph.is_connected(vertex.id, not_neighbor_vertex.id)) {
            to_vertices_no_neighbors.push_back(not_neighbor_vertex);
          }
        }

        if (to_vertices_no_neighbors.size() && distribution(generate)) {
          graph.add_edge(
              vertex.id,
              to_vertices_no_neighbors[rand() % to_vertices_no_neighbors.size()]
                  .id);
        }
      }
    }
  }

  void generate_red_edges(Graph& graph) {
    const auto& depth = params_.depth();
    std::random_device random_device;
    std::mt19937 generate(random_device());
    std::bernoulli_distribution distribution(0.33);
    for (auto& vertex : graph.vertices()) {
      if (depth - vertex.depth >= 2) {
        const auto& to_vertices = graph.get_vertices_on_depth(vertex.depth + 2);
        if (to_vertices.size() && distribution(generate)) {
          graph.add_edge(vertex.id,
                         to_vertices[rand() % to_vertices.size()].id);
        }
      }
    }
  }

  const Graph generate() {
    auto graph = Graph();
    graph.add_vertex();
    generate_grey_edges(graph);
    generate_green_edges(graph);
    generate_yellow_edges(graph);
    generate_red_edges(graph);
    return graph;
  }

 private:
  const Params params_ = Params();
};

const int handle_depth_input() {
  int depth;
  std::cout << "Please, enter the depth of your graph: ";
  std::cin >> depth;
  return depth;
}

const int handle_new_vertices_count_input() {
  int new_vertices_count;
  std::cout
      << "Please, enter count of new neighbors for each vertex of your graph: ";
  std::cin >> new_vertices_count;
  return new_vertices_count;
}

int main() {
  const int depth = handle_depth_input();
  const int new_vertices_count = handle_new_vertices_count_input();

  const auto params = GraphGenerator::Params(depth, new_vertices_count);
  auto generator = GraphGenerator(params);

  const auto graph = generator.generate();

  const auto graph_json = graph_printing::print_graph(graph);
  std::cout << graph_json << std::endl;
  write_to_file(graph_json, "graph.json");

  return 0;
}
