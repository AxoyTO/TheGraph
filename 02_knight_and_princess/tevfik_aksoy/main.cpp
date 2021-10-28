// Все комментарии удалю ближе к PR approve

#include <cassert>
#include <filesystem>
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

constexpr auto INVALID_ID = -1;

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
}  // namespace validation

bool probability(float condition) {
  std::random_device rd;
  std::mt19937 mt(rd());
  std::uniform_real_distribution<float> probability(0.0, 100.0);
  if (probability(mt) >= condition)
    return true;
  else
    return false;
}

VertexId get_random_vertex_id(const vector<VertexId>& vertices) {
  std::uniform_int_distribution<int> random_vertex_distribution(
      vertices[0], vertices[vertices.size() - 1]);
  std::random_device rd;
  std::mt19937 mt(rd());
  return random_vertex_distribution(mt);
}

struct Vertex {
 public:
  const VertexId id{};
  VertexDepth depth = 0;

  explicit Vertex(const VertexId& id) : id(id) {}

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

class Graph {
 public:
  VertexId insert_vertex() {
    const auto id = get_new_vertex_id_();
    vertices_.emplace_back(id);
    if (id == 0) {
      depth_map_[0].push_back(0);
    }
    return id;
  }
  void insert_edge(const VertexId& source,
                   const VertexId& destination,
                   const Edge::Color& color = Edge::Color::Gray) {
    assert(!are_vertices_connected(source, destination) &&
           "Vertices are already connected!");
    const int edge_id = get_new_edge_id_();
    edges_.emplace_back(source, destination, edge_id, color);

    vertices_[source].add_edge_id(edge_id);
    if (color != Edge::Color::Green) {
      vertices_[destination].add_edge_id(edge_id);
      if (color == Edge::Color::Gray) {
        const auto depth = vertices_[source].depth + 1;
        vertices_[destination].depth = depth;
        depth_map_[depth].emplace_back(destination);
      }
    }
  }

  bool are_vertices_connected(const VertexId& source,
                              const VertexId& destination) {
    int count = 0;
    for (const auto& edge : edges_) {
      if (edge.source == source && edge.destination == destination) {
        count++;
      }
      if (count > 1)
        return true;
    }

    return false;
  }

  void initialize_and_update_depth_map(const int max_depth) {
    // Max Depth +1 because depth indexing starts from 0
    depth_map_.resize(max_depth + 1);
  }

  void update_depth_map() {
    // Need to update the depth map after generation of all vertices
    // if input max_depth couldn't be reached
    // this function is being called at the beginning and end of the
    // generate_gray_edges_vertices function
    for (auto it = depth_map_.rbegin(); it != depth_map_.rend(); it++) {
      if (it->size())
        break;
      else
        depth_map_.pop_back();
    }
    // Reverse iterating through the depth_map_ to
    // remove all the unreached depths
  }

  int depth() const { return depth_map_.size() - 1; }

  const vector<Vertex>& get_vertices() const { return vertices_; }

  const vector<VertexId>& get_vertices_in_depth(
      const VertexDepth& depth) const {
    return depth_map_.at(depth);
  }

  VertexId get_source_of_vertex(const VertexId& vertex) const {
    for (const auto& vertex_edge : vertices_[vertex].get_edge_ids()) {
      if (edges_[vertex_edge].color == Edge::Color::Gray)
        return edges_[vertex_edge].source;
    }
    return INVALID_ID;
  }

  std::string to_JSON() const {
    std::string json_string;
    json_string += "{\n\"vertices\": [\n";
    for (int i = 0; i < vertices_.size(); i++) {
      json_string += vertices_[i].to_JSON();
      if (i + 1 == vertices_.size()) {
        json_string += "\n  ],\n";
      } else {
        json_string += ",\n";
      }
    }

    json_string += "\"edges\": [\n";
    for (int i = 0; i < edges_.size(); i++) {
      json_string += edges_[i].to_JSON();
      if (i + 1 == edges_.size()) {
        json_string += "\n";
      } else {
        json_string += ",\n";
      }
    }
    json_string += "  ]\n}\n";
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
  graph.initialize_and_update_depth_map(max_depth);
  float condition = 0;
  graph.insert_vertex();

  for (VertexDepth depth = 0; depth < max_depth; depth++) {
    for (const auto& source : graph.get_vertices_in_depth(depth)) {
      for (int j = 0; j < new_vertices_num; j++) {
        if (probability(condition)) {
          const VertexId new_vertex = graph.insert_vertex();
          graph.insert_edge(source, new_vertex, Edge::Color::Gray);
        }
      }
    }
    condition += 100 / (float)max_depth;
  }

  graph.update_depth_map();
  if (max_depth != graph.depth()) {
    std::cout << "\nMax depth couldn't be reached. Depth of final vertex: "
              << graph.depth() << "\n";
  }
}

void generate_green_edges(Graph& graph) {
  int condition = 90;
  for (const auto& vertex : graph.get_vertices()) {
    if (probability(condition)) {
      graph.insert_edge(vertex.id, vertex.id, Edge::Color::Green);
    }
  }
}

void generate_blue_edges(Graph& graph) {
  float condition = 75;
  for (int depth = 0; depth < graph.depth(); depth++) {
    const auto& vertices_in_depth = graph.get_vertices_in_depth(depth);
    for (VertexId j = 0; j < vertices_in_depth.size() - 1; j++) {
      const auto source = vertices_in_depth[j];
      const auto destination = vertices_in_depth[j + 1];
      if (probability(condition)) {
        graph.insert_edge(source, destination, Edge::Color::Blue);
      }
    }
  }
}

vector<VertexId> filter_unconnected(const VertexId& vertex,
                                    const vector<VertexId>& vertices_to_filter,
                                    const Graph& graph) {
  vector<VertexId> filtered_vertices;
  for (const auto& v : vertices_to_filter) {
    if (graph.get_source_of_vertex(v) != vertex) {
      filtered_vertices.push_back(v);
    }
  }
  return filtered_vertices;
}
void generate_yellow_edges(Graph& graph) {
  for (VertexDepth depth = 0; depth < graph.depth(); depth++) {
    float condition = 100 - depth * (100 / (float)(graph.depth() - 1));
    const auto& vertices = graph.get_vertices_in_depth(depth);
    const auto& vertices_next = graph.get_vertices_in_depth(depth + 1);
    for (const auto& vertex : vertices) {
      if (probability(condition)) {
        const auto& filtered_next_vertices =
            filter_unconnected(vertex, vertices_next, graph);
        VertexId next_random_vertex_id;
        bool break_flag = false;
        do {
          if (filtered_next_vertices.size() == 0) {
            // All the vertices in the next depth are created by this
            // vertex, so we break out of the infinite loop
            break_flag = true;
            break;
          }
          next_random_vertex_id = get_random_vertex_id(filtered_next_vertices);
        } while (graph.get_source_of_vertex(next_random_vertex_id) == vertex);
        if (!break_flag)
          // if we've broken out, then there is no random vertex generated
          graph.insert_edge(vertex, next_random_vertex_id, Edge::Color::Yellow);
      }
    }
  }
}

void generate_red_edges(Graph& graph) {
  float condition = 67;
  for (VertexDepth depth = 0; depth < graph.depth() - 1; depth++) {
    const auto& vertices = graph.get_vertices_in_depth(depth);
    const auto& vertices_next = graph.get_vertices_in_depth(depth + 2);
    for (const auto& vertex : vertices) {
      if (probability(condition)) {
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
