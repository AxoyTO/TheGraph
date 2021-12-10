#include <algorithm>
#include <array>
#include <ctime>
#include <fstream>
#include <iostream>
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
const double kGreenEdgesProbability = 10.0;
const double kRedEdgesProbability = 33.0;

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

  void add_vertex() {
    if (vertices_on_depth_.empty()) {
      vertices_on_depth_.emplace(0, vector<VertexId>());
    }
    const VertexId new_vertex_id = get_new_vertex_id();
    vertices_.emplace(new_vertex_id, new_vertex_id);
    vertices_on_depth_.at(0).push_back(new_vertex_id);
  }

  void add_edge(const VertexId& from_vertex_id, const VertexId& to_vertex_id) {
    const EdgeId new_edge_id = get_new_edge_id();
    const auto& new_edge_color =
        define_edge_color(from_vertex_id, to_vertex_id);
    edges_.emplace_back(new_edge_id, from_vertex_id, to_vertex_id,
                        new_edge_color);
    vertices_.at(from_vertex_id).add_edge_id(new_edge_id);
    if (new_edge_color != Edge::Color::Green) {
      vertices_.at(to_vertex_id).add_edge_id(new_edge_id);
    }
    if (new_edge_color == Edge::Color::Gray) {
      const auto new_vertex_depth = vertices_.at(from_vertex_id).depth + 1;
      vertices_.at(to_vertex_id).depth = new_vertex_depth;
      if (vertices_on_depth_.size() - 1 < new_vertex_depth) {
        vertices_on_depth_.emplace(new_vertex_depth, vector<VertexId>());
      } else {
        vertices_on_depth_.at(new_vertex_depth).push_back(to_vertex_id);
      }
      for (auto vertex_id = vertices_on_depth_.at(0).begin();
           vertex_id != vertices_on_depth_.at(0).end(); vertex_id++) {
        if (to_vertex_id == *vertex_id) {
          vertices_on_depth_.at(0).erase(vertex_id);
          break;
        }
      }
    }
  };

  Depth get_vertex_depth(const VertexId& vertex_id) const {
    return vertices_.at(vertex_id).depth;
  }

  bool is_connected(const VertexId& from_vertex_id,
                    const VertexId& to_vertex_id) const {
    for (const auto& edge : edges_) {
      if ((edge.from_vertex_id == from_vertex_id &&
           edge.to_vertex_id == to_vertex_id) ||
          (edge.to_vertex_id == from_vertex_id &&
           edge.from_vertex_id == to_vertex_id)) {
        return true;
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
    if (get_vertex_edges(to_vertex_id).size() == 0) {
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

  const vector<EdgeId>& get_vertex_edges(const VertexId& vertex_id) const {
    return vertices_.at(vertex_id).get_edge_ids();
  }

  const std::unordered_map<VertexId, Vertex>& get_vertices() const {
    return vertices_;
  }

  const std::unordered_map<Depth, vector<VertexId>>& get_vertices_on_depth()
      const {
    return vertices_on_depth_;
  }

  const vector<Edge>& get_edges() const { return edges_; }

 private:
  std::unordered_map<VertexId, Vertex> vertices_;
  std::unordered_map<Depth, vector<VertexId>> vertices_on_depth_;
  vector<Edge> edges_;
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
  string graph_output = "{\n\t\"vertices\": [\n\t\t";
  for (const auto& [key, vertex] : graph.get_vertices()) {
    graph_output += "{" + print_vertex(vertex) + "},\n\t\t";
  }
  graph_output = graph_output.substr(0, graph_output.size() - 4) +
                 "\n\t],\n\t\"edges\": [\n\t\t";
  for (const auto& edge : graph.get_edges()) {
    graph_output += "{" + print_edge(edge) + "}";
    if (edge.id != graph.get_edges().back().id) {
      graph_output += ",\n\t\t";
    } else {
      graph_output += "\n\t";
    }
  }
  graph_output += "]\n}\n";
  return graph_output;
}

}  // namespace graph_printing

bool is_generated(double prob) {
  srand(time(0));
  return (rand() % 100) <= prob;
}

const VertexId& get_random_vertex_id(const vector<VertexId>& vertices) {
  srand(time(0));
  const int random_id = rand() % vertices.size();
  return vertices[random_id];
}

void generate_gray_edges(Graph& graph, int depth, int new_vertices_num) {
  graph.add_vertex();
  VertexId new_child_vertex_id = 1;
  for (int i = 0; i < depth; i++) {
    double step = 100 / depth;
    double prob = 100 - step * i;
    if (i < graph.get_vertices_on_depth().size()) {
      vector<VertexId> temp_vertices_on_depth =
          graph.get_vertices_on_depth().at(i);
      for (const auto& from_vertex_id : temp_vertices_on_depth) {
        for (int k = 0; k < new_vertices_num; k++) {
          if (is_generated(prob)) {
            graph.add_vertex();
            graph.add_edge(from_vertex_id, new_child_vertex_id);
            new_child_vertex_id++;
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
  const auto& graph_depth = graph.get_vertices_on_depth().size();
  for (const auto& [depth, vertices] : graph.get_vertices_on_depth()) {
    if (depth != graph.get_vertices_on_depth().size() - 1) {
      for (const auto& from_vertex_id :
           graph.get_vertices_on_depth().at(depth)) {
        const double prob = graph.get_vertices().at(from_vertex_id).depth *
                            (100 / (graph_depth - 1));
        if (is_generated(prob)) {
          vector<VertexId> vertices_on_deeper_depth;
          for (const auto& to_vertex_id :
               graph.get_vertices_on_depth().at(depth + 1)) {
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
  for (const auto& [depth, vertices] : graph.get_vertices_on_depth()) {
    if (depth < graph.get_vertices_on_depth().size() - 2) {
      for (const auto& from_vertex_id :
           graph.get_vertices_on_depth().at(depth)) {
        if (is_generated(kRedEdgesProbability)) {
          graph.add_edge(from_vertex_id,
                         get_random_vertex_id(
                             graph.get_vertices_on_depth().at(depth + 2)));
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

  cout << depth << std::endl;
  const auto params = GraphGenerator::Params(depth, new_vertices_count);
  const auto generator = GraphGenerator(params);
  const auto graph = generator.generate();

  const auto graph_json = graph_printing::print_graph(graph);
  cout << graph_json << std::endl;
  write_to_file(graph_json, "graph.json");
}
