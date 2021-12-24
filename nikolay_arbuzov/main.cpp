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

constexpr int kNewVertexDepth = 0;

class Graph {
 public:
  using Depth = int;

  struct Vertex {
    explicit Vertex(const VertexId& _id) : id(_id){};
    VertexId id = 0;
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
    set_vertex_depth(new_vertex_id, kNewVertexDepth);
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
    return vertices_depth_.at(vertex_id);
  }

  bool is_connected(const VertexId& from_vertex_id,
                    const VertexId& to_vertex_id) const {
    if (from_vertex_id == to_vertex_id) {
      const auto& edge_ids = adjacency_list_.at(from_vertex_id);
      for (const auto& edge_id : edge_ids) {
        if (get_edge(edge_id).color == Edge::Color::Green) {
          return true;
        }
      }
    } else {
      const auto& from_edge_set = adjacency_list_.at(from_vertex_id);
      const auto& to_edge_set = adjacency_list_.at(to_vertex_id);
      std::set<EdgeId> intersection;
      std::set_intersection(from_edge_set.begin(), from_edge_set.end(),
                            to_edge_set.begin(), to_edge_set.end(),
                            std::inserter(intersection, intersection.begin()));
      if (intersection.size()) {
        return true;
      }
    }
    return false;
  }

  Edge::Color determine_color(const VertexId& from_vertex_id,
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

  Edge& add_edge(const VertexId& from_vertex_id, const VertexId& to_vertex_id) {
    assert(has_vertex(from_vertex_id));
    assert(has_vertex(to_vertex_id));
    const auto& new_edge_color = determine_color(from_vertex_id, to_vertex_id);

    EdgeId new_edge_id = get_new_edge_id();

    adjacency_list_[from_vertex_id].insert(new_edge_id);
    if (from_vertex_id != to_vertex_id)
      adjacency_list_[to_vertex_id].insert(new_edge_id);

    auto& new_edge = edges_.emplace_back(new_edge_id, new_edge_color,
                                         from_vertex_id, to_vertex_id);

    if (new_edge_color == Edge::Color::Grey) {
      const auto from_vertex_depth = get_vertex_depth(from_vertex_id);
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

  const std::vector<VertexId>& get_vertex_ids_on_depth(
      const Depth& depth) const {
    return depth_map_.at(depth);
  }

  const std::vector<Vertex>& vertices() const { return vertices_; }
  const std::vector<Edge>& edges() const { return edges_; }
  Depth depth() const { return depth_map_.size(); }

  const std::set<EdgeId>& connected_edge_ids(const VertexId& id) const {
    return adjacency_list_.at(id);
  }

 private:
  std::vector<Edge> edges_;
  std::vector<Vertex> vertices_;
  std::unordered_map<VertexId, std::set<EdgeId>> adjacency_list_;
  std::unordered_map<VertexId, Depth> vertices_depth_;
  std::vector<std::vector<VertexId>> depth_map_;
  EdgeId edges_id_counter_ = 0;
  VertexId vertex_id_counter_ = 0;

  void set_vertex_depth(const VertexId& vertex_id, const Depth& depth) {
    if (depth && vertex_id && !vertices_depth_[vertex_id]) {
      depth_map_[vertices_depth_[vertex_id]].erase(
          std::find(depth_map_[vertices_depth_[vertex_id]].begin(),
                    depth_map_[vertices_depth_[vertex_id]].end(), vertex_id));
    }
    if (!depth_map_.size() || (depth_map_.size() - 1 < depth)) {
      depth_map_.emplace_back();
    }
    vertices_depth_[vertex_id] = depth;
    depth_map_[depth].push_back(vertex_id);
  }

  VertexId get_new_vertex_id() { return vertex_id_counter_++; }
  EdgeId get_new_edge_id() { return edges_id_counter_++; }
};

namespace graph_printing {
std::string print_edge_color(const Graph::Edge::Color& color) {
  switch (color) {
    case Graph::Edge::Color::Grey:
      return "grey";
    case Graph::Edge::Color::Green:
      return "green";
    case Graph::Edge::Color::Yellow:
      return "yellow";
    case Graph::Edge::Color::Red:
      return "red";
    default:
      throw std::runtime_error("No such color");
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
  result_stream << "\t\t\t\"depth\": " << graph.get_vertex_depth(vertex.id)
                << std::endl;
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
constexpr float kGreenProbability = 0.1;
constexpr float kRedProbability = 0.33;
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

  bool can_generate_vertex(float probability) {
    std::random_device random_device;
    std::mt19937 generate(random_device());
    std::bernoulli_distribution distribution(probability);
    return distribution(generate);
  }
  VertexId get_random_vertex_id(const std::vector<VertexId>& vertex_ids) {
    std::random_device random_device;
    std::mt19937 generator(random_device());
    std::uniform_int_distribution<> distribution(0, vertex_ids.size() - 1);
    return vertex_ids[distribution(generator)];
  }
  explicit GraphGenerator(const Params& params = Params()) : params_(params) {}

  void generate_grey_edges(Graph& graph) {
    const auto& depth = params_.depth();
    const auto& new_vertices_count = params_.new_vertices_count();
    for (Graph::Depth current_depth = 0; current_depth < depth;
         ++current_depth) {
      // Not const reference, because I need to change depth in "for"
      auto vertices_on_current_depth =
          graph.get_vertex_ids_on_depth(current_depth);
      for (const auto& vertex_id : vertices_on_current_depth) {
        for (int new_vertices_number = 0;
             new_vertices_number < new_vertices_count; ++new_vertices_number) {
          if (can_generate_vertex(float(depth - current_depth) / depth)) {
            const auto& new_vertex = graph.add_vertex();
            graph.add_edge(vertex_id, new_vertex.id);
          }
        }
      }
    }
  }

  void generate_green_edges(Graph& graph) {
    const auto& vertices = graph.vertices();
    for (const auto& vertex : vertices) {
      if (can_generate_vertex(kGreenProbability))
        graph.add_edge(vertex.id, vertex.id);
    }
  }

  void generate_yellow_edges(Graph& graph) {
    const auto& depth = params_.depth();
    for (Graph::Depth current_depth = 0; current_depth < depth;
         ++current_depth) {
      // Not const reference, because I need to change depth in "for"
      auto vertices_on_current_depth =
          graph.get_vertex_ids_on_depth(current_depth);
      for (const auto& vertex_id : vertices_on_current_depth) {
        const auto& to_vertex_ids =
            graph.get_vertex_ids_on_depth(current_depth + 1);
        auto to_vertex_ids_no_neighbors = std::vector<VertexId>();

        for (const auto& not_neighbor_vertex_id : to_vertex_ids) {
          if (!graph.is_connected(vertex_id, not_neighbor_vertex_id)) {
            to_vertex_ids_no_neighbors.push_back(not_neighbor_vertex_id);
          }
        }

        if (to_vertex_ids_no_neighbors.size() &&
            can_generate_vertex(
                float(1) - (float(depth - 1 - current_depth) / (depth - 1)))) {
          graph.add_edge(vertex_id,
                         get_random_vertex_id(to_vertex_ids_no_neighbors));
        }
      }
    }
  }

  void generate_red_edges(Graph& graph) {
    const auto& depth = params_.depth();
    for (auto& vertex : graph.vertices()) {
      if (depth - graph.get_vertex_depth(vertex.id) >= 2) {
        const auto& to_vertex_ids = graph.get_vertex_ids_on_depth(
            graph.get_vertex_depth(vertex.id) + 2);
        if (to_vertex_ids.size() && can_generate_vertex(kRedProbability)) {
          graph.add_edge(vertex.id, get_random_vertex_id(to_vertex_ids));
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
