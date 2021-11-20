#include <cassert>
#include <fstream>
#include <iostream>
#include <random>
#include <string>
#include <vector>

using std::string;
using std::to_string;
using std::vector;

using VertexId = int;
using EdgeId = int;
using VertexDepth = int;

struct Edge;
struct Vertex;

constexpr float GREEN_EDGE_PROBABILITY = 0.1;
constexpr float BLUE_EDGE_PROBABILITY = 0.25;
constexpr float RED_EDGE_PROBABILITY = 0.33;

namespace validation {
bool edge_id_exists_in_vertex(const EdgeId& edge_id,
                              const vector<EdgeId>& edge_ids) {
  for (const auto& edge : edge_ids) {
    if (edge_id == edge) {
      return true;
    }
  }
  return false;
}

bool does_vertex_exist(const VertexId& id, const vector<Vertex>& vertices);
}  // namespace validation

float get_random_probability() {
  std::random_device rd;
  std::mt19937 mt(rd());
  std::uniform_real_distribution<float> probability(0.0, 1);
  return probability(mt);
}

VertexId get_random_vertex_id(const vector<VertexId>& vertices) {
  std::random_device rd;
  std::mt19937 mt(rd());
  std::uniform_int_distribution<int> random_vertex_distribution(
      0, vertices.size() - 1);
  return vertices[random_vertex_distribution(mt)];
}

struct Vertex {
 public:
  const VertexId id{};
  VertexDepth depth = 0;

  explicit Vertex(const VertexId& _id) : id(_id) {}

  void add_edge_id(const EdgeId& _id) {
    assert(!validation::edge_id_exists_in_vertex(_id, edge_ids_) &&
           "Edge already exists in vertex!");
    edge_ids_.push_back(_id);
  }
  const vector<EdgeId>& get_edge_ids() const { return edge_ids_; }

  std::string to_JSON() const {
    std::string json_string;
    json_string += "\t{ \"id\": " + to_string(id) + ", \"edge_ids\": [";
    for (int i = 0; i < edge_ids_.size(); i++) {
      json_string += to_string(edge_ids_[i]);
      if (i + 1 != edge_ids_.size())
        json_string += ", ";
    }
    json_string += "], \"depth\": " + to_string(depth) + "}";
    return json_string;
  }

 private:
  vector<EdgeId> edge_ids_;
};

struct Edge {
 public:
  enum class Color { Gray, Green, Blue, Yellow, Red };
  const EdgeId id{};
  const Color color{};
  const VertexId source{};
  const VertexId destination{};

  Edge(const VertexId& _source,
       const VertexId& _destination,
       const VertexId& _id,
       const Color& _color)
      : id(_id), color(_color), source(_source), destination(_destination) {}

  std::string to_JSON() const;
};

std::string color_to_string(const Edge::Color& color) {
  switch (color) {
    case Edge::Color::Gray:
      return "gray";
    case Edge::Color::Green:
      return "green";
    case Edge::Color::Blue:
      return "blue";
    case Edge::Color::Yellow:
      return "yellow";
    case Edge::Color::Red:
      return "red";
  }
}

std::string Edge::to_JSON() const {
  std::string json_string;
  json_string += "\t{ \"id\": " + to_string(id) + ", \"vertex_ids\": [" +
                 to_string(source) + ", " + to_string(destination) +
                 "], \"color\": \"" + color_to_string(color) + "\" }";
  return json_string;
}

bool validation::does_vertex_exist(const VertexId& id,
                                   const vector<Vertex>& vertices) {
  for (const auto& vertex : vertices)
    if (vertex.id == id) {
      return true;
    }
  return false;
}

class Graph {
 public:
  VertexId insert_vertex() {
    const auto id = get_new_vertex_id_();
    vertices_.emplace_back(id);
    if (id == 0) {
      auto& first_depth_layer = depth_map_.emplace_back();
      first_depth_layer.push_back(id);
    }
    return id;
  }
  void insert_edge(const VertexId& source,
                   const VertexId& destination,
                   const Edge::Color& color = Edge::Color::Gray) {
    assert(!are_vertices_connected(source, destination) &&
           "Vertices are already connected!");
    assert(does_color_match_vertices(vertices_[source], vertices_[destination],
                                     color) &&
           "Color does not match the vertices!");
    const int edge_id = get_new_edge_id_();
    edges_.emplace_back(source, destination, edge_id, color);

    vertices_[source].add_edge_id(edge_id);
    if (color != Edge::Color::Green) {
      vertices_[destination].add_edge_id(edge_id);
      if (color == Edge::Color::Gray) {
        const auto depth = vertices_[source].depth + 1;
        vertices_[destination].depth = depth;
        if (depth_map_.size() == depth) {
          depth_map_.emplace_back();
        }
        depth_map_[depth].emplace_back(destination);
      }
    }
  }

  bool does_color_match_vertices(const Vertex& source,
                                 const Vertex& destination,
                                 const Edge::Color& color) const {
    switch (color) {
      case Edge::Color::Gray:
        if (source.get_edge_ids().size() == 0 ||
            destination.get_edge_ids().size() == 0)
          return true;
      case Edge::Color::Green:
        if (source.id == destination.id)
          return true;
      case Edge::Color::Blue:
        if (source.depth == destination.depth) {
          for (int i = 0; i < depth_map_[source.depth].size() - 1; i++) {
            auto first = depth_map_[source.depth][i];
            auto second = depth_map_[source.depth][i + 1];
            if ((source.id == first && destination.id == second) ||
                (destination.id == first && source.id == second))
              return true;
          }
        }
      case Edge::Color::Yellow:
        if (source.depth == destination.depth - 1)
          return true;
      case Edge::Color::Red:
        if (source.depth == destination.depth - 2)
          return true;
    }
    return false;
  }

  bool are_vertices_connected(const VertexId& source,
                              const VertexId& destination) const {
    assert(validation::does_vertex_exist(source, vertices_) &&
           "Source vertex doesn't exist!");
    assert(validation::does_vertex_exist(destination, vertices_) &&
           "Destination vertex doesn't exist!");

    const auto& source_vertex_edges = vertices_[source].get_edge_ids();
    const auto& destination_vertex_edges =
        vertices_[destination].get_edge_ids();
    for (const auto& edge_of_source_vertex : source_vertex_edges)
      if (source == destination) {
        if (edges_[edge_of_source_vertex].source ==
            edges_[edge_of_source_vertex].destination)
          return true;
      } else {
        for (const auto& edge_of_destination_vertex : destination_vertex_edges)
          if (edge_of_source_vertex == edge_of_destination_vertex)
            return true;
      }
    return false;
  }

  int depth() const { return depth_map_.size() - 1; }

  const vector<Vertex>& get_vertices() const { return vertices_; }

  const vector<VertexId>& get_vertices_in_depth(
      const VertexDepth& depth) const {
    return depth_map_.at(depth);
  }

  std::string to_JSON() const {
    std::string json_string;
    json_string += "{\n\"vertices\": [\n";
    for (int i = 0; i < vertices_.size(); i++) {
      json_string += vertices_[i].to_JSON();
      if (i + 1 != vertices_.size())
        json_string += ",\n";
    }
    json_string += "\n  ],\n";

    json_string += "\"edges\": [\n";
    for (int i = 0; i < edges_.size(); i++) {
      json_string += edges_[i].to_JSON();
      if (i + 1 != edges_.size())
        json_string += ",\n";
    }
    json_string += "\n  ]\n}\n";
    return json_string;
  }

 private:
  vector<Edge> edges_;
  vector<Vertex> vertices_;
  vector<vector<VertexId>> depth_map_;
  VertexId vertex_id_counter_ = 0;
  EdgeId edge_id_counter_ = 0;

  VertexId get_new_vertex_id_() { return vertex_id_counter_++; }
  EdgeId get_new_edge_id_() { return edge_id_counter_++; }
};

void generate_vertices_and_gray_edges(Graph& graph,
                                      const VertexDepth& max_depth,
                                      const int new_vertices_num) {
  graph.insert_vertex();

  for (VertexDepth depth = 0; depth < max_depth; depth++) {
    bool is_new_vertex_generated = false;
    const float probability = (float)depth / (float)max_depth;
    for (const auto& source : graph.get_vertices_in_depth(depth)) {
      for (int j = 0; j < new_vertices_num; j++) {
        if (get_random_probability() > probability) {
          is_new_vertex_generated = true;
          const VertexId new_vertex = graph.insert_vertex();
          graph.insert_edge(source, new_vertex, Edge::Color::Gray);
        }
      }
    }
    if (!is_new_vertex_generated)
      break;
  }

  if (max_depth != graph.depth()) {
    std::cout << "Max depth couldn't be reached. Depth of final vertex: "
              << graph.depth() << "\n";
  }
}

void generate_green_edges(Graph& graph) {
  for (const auto& vertex : graph.get_vertices()) {
    if (get_random_probability() < GREEN_EDGE_PROBABILITY) {
      graph.insert_edge(vertex.id, vertex.id, Edge::Color::Green);
    }
  }
}

void generate_blue_edges(Graph& graph) {
  for (int depth = 0; depth < graph.depth(); depth++) {
    const auto& vertices_in_depth = graph.get_vertices_in_depth(depth);
    for (VertexId j = 0; j < vertices_in_depth.size() - 1; j++) {
      const auto source = vertices_in_depth[j];
      const auto destination = vertices_in_depth[j + 1];
      if (get_random_probability() < BLUE_EDGE_PROBABILITY) {
        graph.insert_edge(source, destination, Edge::Color::Blue);
      }
    }
  }
}

vector<VertexId> filter_connected_vertices(const VertexId& id,
                                           const vector<VertexId>& vertex_ids,
                                           const Graph& graph) {
  vector<VertexId> result;
  for (const auto& vertex_id : vertex_ids) {
    if (!graph.are_vertices_connected(id, vertex_id)) {
      result.push_back(vertex_id);
    }
  }
  return result;
}
void generate_yellow_edges(Graph& graph) {
  for (VertexDepth depth = 0; depth < graph.depth(); depth++) {
    float probability = 1 - depth * (1 / (float)(graph.depth() - 1));
    const auto& vertices = graph.get_vertices_in_depth(depth);
    const auto& vertices_next = graph.get_vertices_in_depth(depth + 1);
    for (const auto& vertex_id : vertices) {
      if (get_random_probability() > probability) {
        const auto& filtered_vertex_ids =
            filter_connected_vertices(vertex_id, vertices_next, graph);
        if (filtered_vertex_ids.size() == 0) {
          continue;
        }
        VertexId random_vertex_id = get_random_vertex_id(filtered_vertex_ids);
        graph.insert_edge(vertex_id, random_vertex_id, Edge::Color::Yellow);
      }
    }
  }
}

void generate_red_edges(Graph& graph) {
  for (VertexDepth depth = 0; depth < graph.depth() - 1; depth++) {
    const auto& vertices = graph.get_vertices_in_depth(depth);
    const auto& vertices_next = graph.get_vertices_in_depth(depth + 2);
    for (const auto& vertex : vertices) {
      if (get_random_probability() < RED_EDGE_PROBABILITY) {
        graph.insert_edge(vertex, get_random_vertex_id(vertices_next),
                          Edge::Color::Red);
      }
    }
  }
}

const Graph generateGraph(const VertexDepth& max_depth,
                          const int new_vertices_num) {
  Graph graph;
  generate_vertices_and_gray_edges(graph, max_depth, new_vertices_num);
  generate_green_edges(graph);
  generate_blue_edges(graph);
  generate_yellow_edges(graph);
  generate_red_edges(graph);

  return graph;
}

int main() {
  int max_depth = 0;
  int new_vertices_num = 0;

  std::cout << "Enter max_depth: ";
  do {
    std::cin >> max_depth;
    if (max_depth < 0)
      std::cerr << "Depth can not be negative!\n"
                   "Enter a non-negative max_depth: ";
  } while (max_depth < 0);
  std::cout << "Enter new_vertices_num: ";
  do {
    std::cin >> new_vertices_num;
    if (new_vertices_num < 0)
      std::cerr << "Number of new vertices created by each vertex can not be "
                   "negative!\nEnter a non-negative new_vertices_num: ";
  } while (new_vertices_num < 0);

  std::cout << "\n";

  const string filename = "./graph.json";
  std::ofstream file(filename, std::ios::out);
  if (!file.is_open())
    std::cerr << "Error opening the file graph.json!\n";
  else {
    const auto graph = generateGraph(max_depth, new_vertices_num);
    file << graph.to_JSON();
    file.close();
  }
  return 0;
}
