#include <algorithm>
#include <array>
#include <chrono>
#include <fstream>
#include <iostream>
#include <random>
#include <string>
#include <unordered_map>
#include <vector>

using std::array;
using std::cin;
using std::cout;
using std::string;
using std::to_string;
using std::vector;
using VertexId = int;
using EdgeId = int;
constexpr double kGreenEdgesProbability = 10.0;
constexpr double kRedEdgesProbability = 33.0;

class Graph {
 public:
  using Depth = int;

  struct Vertex {
   public:
    const VertexId id;
    Depth depth = 0;

    explicit Vertex(const VertexId& _id) : id(_id) {}

    void add_edge_id(const EdgeId& id) { edge_ids_.push_back(id); }

    const vector<EdgeId>& get_edge_ids() const { return edge_ids_; }

   private:
    vector<EdgeId> edge_ids_;
  };

  struct Edge {
    enum class Color { Gray, Green, Yellow, Red };

    const VertexId from_vertex_id;
    const VertexId to_vertex_id;
    const EdgeId id;
    const Color color;

    Edge(const EdgeId& _id,
         const VertexId& _from_vertex_id,
         const VertexId& _to_vertex_id,
         const Color& _color)
        : id(_id),
          from_vertex_id(_from_vertex_id),
          to_vertex_id(_to_vertex_id),
          color(_color) {}
  };

  VertexId add_vertex() {
    if (vertices_on_depth_.empty()) {
      vertices_on_depth_.push_back(vector<VertexId>());
    }
    const VertexId new_vertex_id = get_new_vertex_id();
    vertices_.emplace(new_vertex_id, new_vertex_id);
    vertices_on_depth_[0].push_back(new_vertex_id);
    return new_vertex_id;
  }

  void add_edge(const VertexId& from_vertex_id, const VertexId& to_vertex_id) {
    const EdgeId new_edge_id = get_new_edge_id();
    const auto& new_edge_color =
        define_edge_color(from_vertex_id, to_vertex_id);
    edges_.emplace(new_edge_id, Edge(new_edge_id, from_vertex_id, to_vertex_id,
                                     new_edge_color));
    get_vertex(from_vertex_id).add_edge_id(new_edge_id);
    if (new_edge_color != Edge::Color::Green) {
      get_vertex(to_vertex_id).add_edge_id(new_edge_id);
    }
    if (new_edge_color == Edge::Color::Gray) {
      const auto new_vertex_depth = get_vertex(from_vertex_id).depth + 1;
      get_vertex(to_vertex_id).depth = new_vertex_depth;
      if (vertices_on_depth_.size() - 1 < new_vertex_depth) {
        vertices_on_depth_.push_back(vector<VertexId>());
      }
      vertices_on_depth_[new_vertex_depth].push_back(to_vertex_id);
      for (auto vertex_id = vertices_on_depth_.at(0).begin();
           vertex_id != vertices_on_depth_[0].end(); vertex_id++) {
        if (to_vertex_id == *vertex_id) {
          vertices_on_depth_[0].erase(vertex_id);
          break;
        }
      }
    }
  };

  Vertex& get_vertex(const VertexId& id) {
    const auto& const_this = *this;

    return const_cast<Vertex&>(const_this.vertices_.at(id));
  }
  const Edge& get_edge(const EdgeId& id) const { return edges_.at(id); }

  Depth get_vertex_depth(const VertexId& vertex_id) const {
    return vertices_.at(vertex_id).depth;
  }

  bool is_connected(const VertexId& from_vertex_id,
                    const VertexId& to_vertex_id) const {
    if (from_vertex_id != to_vertex_id) {
      for (const auto& edge_id_1 : get_vertex_edge_ids(from_vertex_id)) {
        for (const auto& edge_id_2 : get_vertex_edge_ids(to_vertex_id)) {
          if (edge_id_1 == edge_id_2) {
            return true;
          }
        }
      }
    } else {
      for (const auto& edge_id : get_vertex_edge_ids(from_vertex_id)) {
        if (get_edge(edge_id).from_vertex_id ==
            get_edge(edge_id).to_vertex_id) {
          return true;
        }
      }
    }
    return false;
  }

  Edge::Color define_edge_color(const VertexId& from_vertex_id,
                                const VertexId& to_vertex_id) const {
    const auto from_vertex_depth = get_vertex_depth(from_vertex_id);
    const auto to_vertex_depth = get_vertex_depth(to_vertex_id);
    if (from_vertex_id == to_vertex_id) {
      return Edge::Color::Green;
    }
    if (get_vertex_edge_ids(to_vertex_id).size() == 0) {
      return Edge::Color::Gray;
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

  const vector<EdgeId>& get_vertex_edge_ids(const VertexId& vertex_id) const {
    return vertices_.at(vertex_id).get_edge_ids();
  }

  const std::unordered_map<VertexId, Vertex>& get_vertices() const {
    return vertices_;
  }

  const vector<vector<VertexId>>& get_vertices_on_depth() const {
    return vertices_on_depth_;
  }

  const vector<VertexId>& get_vertex_ids_on_depth(Depth depth) const {
    return vertices_on_depth_[depth];
  }

  const std::unordered_map<EdgeId, Edge>& get_edges() const { return edges_; }

  Depth depth() const { return vertices_on_depth_.size() - 1; }

 private:
  std::unordered_map<VertexId, Vertex> vertices_;
  vector<vector<VertexId>> vertices_on_depth_;
  std::unordered_map<EdgeId, Edge> edges_;
  VertexId vertex_id_counter_ = 0;
  EdgeId edge_id_counter_ = 0;

  VertexId get_new_vertex_id() { return vertex_id_counter_++; }

  EdgeId get_new_edge_id() { return edge_id_counter_++; }
};

namespace graph_printing {
string print_edge_color(const Graph::Edge::Color& color) {
  switch (color) {
    case Graph::Edge::Color::Gray:
      return "gray";
    case Graph::Edge::Color::Green:
      return "green";
    case Graph::Edge::Color::Yellow:
      return "yellow";
    case Graph::Edge::Color::Red:
      return "red";
  }
  throw std::runtime_error("Failed to determine color");
}

string print_vertex(const Graph::Vertex& vertex) {
  string vertex_output =
      "\"id\": " + to_string(vertex.id) + ", " + "\"edge_ids\": [";
  for (const auto& edge_id : vertex.get_edge_ids()) {
    vertex_output += to_string(edge_id);
    if (edge_id != vertex.get_edge_ids().back()) {
      vertex_output += ", ";
    }
  }
  vertex_output += "], \"depth\": " + to_string(vertex.depth);
  return vertex_output;
}
string print_edge(const Graph::Edge& edge) {
  return "\"id\": " + std::to_string(edge.id) + ", " + "\"vertex_ids\": [" +
         to_string(edge.from_vertex_id) + ", " + to_string(edge.to_vertex_id) +
         "]" + ", " + "\"color\": \"" + print_edge_color(edge.color) + "\"";
}
string print_graph(const Graph& graph) {
  string graph_output = "{\n\t\"depth\": " + to_string(graph.depth()) +
                        ",\n\t\"vertices\": [\n\t\t";
  for (const auto& [key, vertex] : graph.get_vertices()) {
    graph_output += "{" + print_vertex(vertex) + "},\n\t\t";
  }
  graph_output = graph_output.substr(0, graph_output.size() - 4) +
                 "\n\t],\n\t\"edges\": [\n\t\t";
  for (const auto& [key, edge] : graph.get_edges()) {
    graph_output += "{" + print_edge(edge) + "},\n\t\t";
  }
  graph_output = graph_output.substr(0, graph_output.size() - 4) + "\n\t]\n}\n";
  return graph_output;
}

}  // namespace graph_printing

bool is_generated(double probability) {
  std::random_device rd;
  std::default_random_engine gen(rd());
  std::bernoulli_distribution is_gen(probability * 0.01);
  return is_gen(gen);
}

const VertexId& get_random_vertex_id(const vector<VertexId>& vertices) {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> random_id(0, vertices.size() - 1);
  return vertices[random_id(gen)];
}

void generate_gray_edges(Graph& graph, int depth, int new_vertices_num) {
  for (int current_depth = 0; current_depth < depth; current_depth++) {
    double step = 100 / (double)depth;
    double probability = 100 - step * current_depth;
    if (current_depth < graph.depth() + 1) {
      //Создаю копию, чтобы не менять объект по которому итерирую
      const auto vertex_ids_on_depth =
          graph.get_vertex_ids_on_depth(current_depth);
      for (const auto& from_vertex_id : vertex_ids_on_depth) {
        for (int k = 0; k < new_vertices_num; k++) {
          if (is_generated(probability)) {
            const auto new_vertex_id = graph.add_vertex();
            graph.add_edge(from_vertex_id, new_vertex_id);
          }
        }
      }
    }
  }
}

void generate_green_edges(Graph& graph) {
  for (const auto& [vertex_id, vertex] : graph.get_vertices()) {
    if (is_generated(kGreenEdgesProbability)) {
      graph.add_edge(vertex_id, vertex_id);
    }
  }
}

void generate_yellow_edges(Graph& graph) {
  for (int current_depth = 0; current_depth <= graph.depth(); current_depth++) {
    if (current_depth != graph.depth()) {
      for (const auto& from_vertex_id :
           graph.get_vertex_ids_on_depth(current_depth)) {
        const double probability =
            (double)graph.get_vertex(from_vertex_id).depth *
            (100 / (double)(graph.depth() - 1));
        if (is_generated(probability)) {
          vector<VertexId> vertices_on_deeper_depth;
          for (const auto& to_vertex_id :
               graph.get_vertex_ids_on_depth(current_depth + 1)) {
            if (!graph.is_connected(from_vertex_id, to_vertex_id)) {
              vertices_on_deeper_depth.push_back(to_vertex_id);
            }
          }

          if (!vertices_on_deeper_depth.empty()) {
            graph.add_edge(from_vertex_id,
                           get_random_vertex_id(vertices_on_deeper_depth));
          }
        }
      }
    }
  }
}

void generate_red_edges(Graph& graph) {
  for (int current_depth = 0; current_depth <= graph.depth(); current_depth++) {
    if (current_depth >= graph.depth() - 1) {
      break;
    } else {
      for (const auto& from_vertex_id :
           graph.get_vertex_ids_on_depth(current_depth)) {
        if (is_generated(kRedEdgesProbability)) {
          graph.add_edge(from_vertex_id,
                         get_random_vertex_id(
                             graph.get_vertex_ids_on_depth(current_depth + 2)));
        }
      }
    }
  }
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

  Graph generate() const {
    auto graph = Graph();
    graph.add_vertex();
    generate_gray_edges(graph, params_.depth(), params_.new_vertices_count());
    generate_green_edges(graph);
    generate_yellow_edges(graph);
    generate_red_edges(graph);
    return graph;
  }

 private:
  const Params params_ = Params();
};

void write_to_file(const string& json, const string& file_name) {
  std::ofstream file;
  file.open(file_name);
  file << json;
}

int handle_depth_input() {
  int depth;
  while (true) {
    cout << "Enter depth of graph:\n";
    cin >> depth;
    if (depth < 0) {
      std::cout << "Incorrect input. Depth must be integer and >= 0\n";
      cin.clear();
    } else {
      break;
    }
  }
  return depth;
}

int handle_new_vertices_count_input() {
  int new_vertices_num;
  bool incorrect_input = true;
  while (true) {
    cout << "Enter max number of vertices for each vertex:\n";
    cin >> new_vertices_num;
    if (new_vertices_num < 0) {
      cout << "Incorrect input. Number of vertices must be integer and >= 0\n";
    } else {
      break;
    }
  }
  return new_vertices_num;
}

int main() {
  const int depth = handle_depth_input();
  const int new_vertices_count = handle_new_vertices_count_input();

  const auto params = GraphGenerator::Params(depth, new_vertices_count);
  const auto generator = GraphGenerator(params);
  const auto graph = generator.generate();

  const auto graph_json = graph_printing::print_graph(graph);
  cout << graph_json << std::endl;
  write_to_file(graph_json, "graph.json");
}
