#include <cassert>
#include <ctime>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <random>
#include <string>
#include <vector>

using std::string;
using std::vector;

using VertexId = int;
using EdgeId = int;
using VertexDepth = int;

struct Edge;
struct Vertex;

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

bool vertex_exists_in_graph(const VertexId& id, const vector<Vertex>& vertices);
bool edge_exists_in_graph(const EdgeId& id, const vector<Edge>& edges);

bool is_vertex_id_valid(const VertexId& id) {
  if (id < 0) {
    return false;
  }
  return true;
}

bool is_edge_id_valid(const EdgeId& id) {
  if (id < 0) {
    return false;
  }
  return true;
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

struct Vertex {
 public:
  const VertexId id;
  const VertexDepth depth;

  Vertex(const VertexId& id, const VertexDepth& depth) : id(id), depth(depth) {}

  void add_edge_id(const EdgeId& _id) {
    assert(!validation::edge_id_exists_in_vertex(_id, edge_ids_) &&
           "Edge already exists in vertex!");
    edge_ids_.push_back(_id);
  }
  const vector<EdgeId>& get_edge_ids() const { return edge_ids_; }
  std::string to_JSON() const {
    std::string json_string;

    json_string += "\t{ \"id\": " + std::to_string(id) + ", \"edge_ids\": [";
    for (int i = 0; i < edge_ids_.size(); i++) {
      json_string += std::to_string(edge_ids_[i]);
      if (i + 1 != edge_ids_.size())
        json_string += ", ";
    }
    json_string += "], \"depth\": " + std::to_string(depth) + "}";
    return json_string;
  }

 private:
  vector<EdgeId> edge_ids_;
};

struct Edge {
 public:
  enum class Color { Gray, Green, Blue, Yellow, Red };
  const EdgeId id;
  const Color color;
  const VertexId source;
  const VertexId destination;

  Edge(const VertexId& _source,
       const VertexId& _destination,
       const VertexId& _id,
       const Color& _color)
      : id(_id), color(_color), source(_source), destination(_destination) {}

  std::string color_to_string(const Edge::Color& _color) const;

  std::string to_JSON() const {
    std::string json_string;
    json_string += "\t{ \"id\": " + std::to_string(id) + ", \"vertex_ids\": [" +
                   std::to_string(source) + ", " + std::to_string(destination) +
                   "], \"color\": \"" + color_to_string(color) + "\" }";
    return json_string;
  }
};

std::string Edge::color_to_string(const Edge::Color& _color) const {
  switch (_color) {
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

bool validation::vertex_exists_in_graph(const VertexId& id,
                                        const vector<Vertex>& vertices) {
  for (const auto& vertex : vertices)
    if (vertex.id == id) {
      return true;
    }
  return false;
}
bool validation::edge_exists_in_graph(const EdgeId& id,
                                      const vector<Edge>& edges) {
  for (const auto& edge : edges)
    if (edge.id == id) {
      return true;
    }
  return false;
}

class Graph {
 public:
  void insert_vertex(const VertexId& vertex, const VertexDepth& depth) {
    assert(validation::is_vertex_id_valid(vertex) && "Vertex id is not valid!");
    assert(!validation::vertex_exists_in_graph(vertex, vertices_) &&
           "Vertex already exists!");
    vertices_.emplace_back(vertex, depth);
  }
  void insert_edge(const VertexId& source,
                   const VertexId& destination,
                   const EdgeId& id,
                   const Edge::Color& color = Edge::Color::Gray) {
    assert(validation::is_edge_id_valid(id) && "Edge id is not valid!");
    assert(!validation::edge_exists_in_graph(id, edges_) &&
           "Edge already exists!");
    assert(validation::is_vertex_id_valid(source) &&
           "Source Vertex id is not valid!");
    assert(validation::is_vertex_id_valid(destination) &&
           "Destination Vertex id is not valid!");
    assert(validation::vertex_exists_in_graph(source, vertices_) &&
           "Source vertex id doesn't exist!");
    assert(validation::vertex_exists_in_graph(destination, vertices_) &&
           "Destination vertex id doesn't exist!");
    assert(!are_vertices_connected(source, destination) &&
           "Vertices are already connected!");

    edges_.emplace_back(source, destination, id, color);
    if (color != Edge::Color::Green) {
      vertices_[source].add_edge_id(id);
      vertices_[destination].add_edge_id(id);
    } else {
      vertices_[source].add_edge_id(id);
    }
  }

  bool are_vertices_connected(const VertexId& source_vertex,
                              const VertexId& destination_vertex) {
    if (source_vertex != destination_vertex) {
      for (const auto& source : vertices_[source_vertex].get_edge_ids()) {
        for (const auto& destination :
             vertices_[destination_vertex].get_edge_ids()) {
          if (source == destination) {
            std::cerr << "Vertices: " << source_vertex << " and "
                      << destination_vertex
                      << " are already connected with edge: " << source << "\n";
            return true;
          }
        }
      }
    }
    return false;
  }

  VertexId source_of_vertex(const VertexId& vertex) const {
    for (const auto& vertex_edge : vertices_[vertex].get_edge_ids()) {
      if (edges_[vertex_edge].color == Edge::Color::Gray)
        return edges_[vertex_edge].source;
    }
    return -1;
  }

  // Returns depth of vertex in the graph
  const VertexDepth depth_of(const VertexId& id) const {
    return vertices_[id].depth;
  }

  const vector<Vertex>& get_vertices() const { return vertices_; }
  const vector<Edge>& get_edges() const { return edges_; }
  
  // Returns total number of vertices in the depth
  int vertices_count_in_depth(const VertexDepth& depth) const {
    if (depth == 0)
      return 1;
    int count = 0;
    for (const auto& vertex : vertices_) {
      if (depth == vertex.depth)
        count++;
    }
    return count;
  }

  // Returns all the vertices in depth(x)
  vector<VertexId> vertices_in_depth(const VertexDepth& depth) {
    vector<VertexId> vertices;
    for (const auto& vertex : vertices_) {
      if (vertex.depth == depth) {
        vertices.push_back(vertex.id);
      }
    }
    return vertices;
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
};

void generate_vertices_and_gray_edges(Graph& graph,
                                      VertexDepth& max_depth,
                                      const int& new_vertices_num);
void generate_green_edges(Graph& graph);
void generate_blue_edges(Graph& graph);

void generate_yellow_edges(Graph& graph,
                           const VertexDepth& max_depth,
                           const int& new_vertices_num);

void generate_red_edges(Graph& graph, const VertexDepth& max_depth);

const Graph generateGraph(VertexDepth& max_depth, const int& new_vertices_num) {
  Graph graph;
  clock_t begin, end;
  double elapsed_time;
  begin = clock();
  std::cout << "Generating vertices and gray edges...   ";
  generate_vertices_and_gray_edges(graph, max_depth, new_vertices_num);
  end = clock();
  elapsed_time = (double)(end - begin) / CLOCKS_PER_SEC;
  std::cout << "Generated vertices and gray edges. Elapsed Time: "
            << elapsed_time << " s.\n";
  std::cout << "Generating green edges...   ";
  begin = clock();
  generate_green_edges(graph);
  end = clock();
  elapsed_time = (double)(end - begin) / CLOCKS_PER_SEC;
  std::cout << "Generated green edges. Elapsed Time: " << elapsed_time
            << " s.\n";
  std::cout << "Generating blue edges...   ";
  begin = clock();
  generate_blue_edges(graph);
  end = clock();
  elapsed_time = (double)(end - begin) / CLOCKS_PER_SEC;
  std::cout << "Generated blue edges. Elapsed Time: " << elapsed_time
            << " s.\n";

  std::cout << "Generating yellow edges...   ";
  begin = clock();
  generate_yellow_edges(graph, max_depth, new_vertices_num);
  end = clock();
  elapsed_time = (double)(end - begin) / CLOCKS_PER_SEC;
  std::cout << "Generated yellow edges. Elapsed Time: " << elapsed_time
            << " s.\n";

  std::cout << "Generating red edges...   ";
  begin = clock();
  generate_red_edges(graph, max_depth);
  end = clock();
  elapsed_time = (double)(end - begin) / CLOCKS_PER_SEC;
  std::cout << "Generated red edges. Elapsed Time: " << elapsed_time
            << " s.\n\n";

  return graph;
}

void generate_vertices_and_gray_edges(Graph& graph,
                                      VertexDepth& max_depth,
                                      const int& new_vertices_num) {
  graph.insert_vertex(0, 0);
  float condition = 0;
  VertexId source_vertex = 0;
  VertexId vertex = 1;
  for (VertexDepth depth = 0; depth < max_depth; depth++) {
    for (int i = 0; i < graph.vertices_count_in_depth(depth); i++) {
      for (int j = 0; j < new_vertices_num; j++) {
        if (probability(condition)) {
          graph.insert_vertex(vertex, depth + 1);
          EdgeId edge_id = graph.get_edges().size();
          graph.insert_edge(source_vertex, vertex, edge_id, Edge::Color::Gray);
          vertex++;
        }
      }
      source_vertex++;
    }
    condition += 100 / (float)max_depth;
  }
  if (max_depth != graph.depth_of(vertex - 1)) {
    std::cout << "\nMax depth couldn't be reached. Depth of final vertex: "
              << graph.depth_of(vertex - 1) << "\n";
    max_depth = graph.depth_of(vertex - 1);
  }
}

void generate_green_edges(Graph& graph) {
  int condition = 90;
  for (const auto& vertex : graph.get_vertices()) {
    if (probability(condition)) {
      EdgeId edge_id = graph.get_edges().size();
      graph.insert_edge(vertex.id, vertex.id, edge_id, Edge::Color::Green);
    }
  }
}

void generate_blue_edges(Graph& graph) {
  float condition = 75;
  for (int i = 0; i < graph.get_vertices().size() - 1; i++) {
    if (graph.depth_of(i) == graph.depth_of(i + 1))
      if (probability(condition)) {
        EdgeId edge_id = graph.get_edges().size();
        graph.insert_edge(i, i + 1, edge_id, Edge::Color::Blue);
      }
  }
}

const int created_by_vertex(const Graph& graph,
                            const VertexId& id,
                            const int& new_vertices_num);

void generate_yellow_edges(Graph& graph,
                           const VertexDepth& max_depth,
                           const int& new_vertices_num) {
  int m = 0, n = 0;
  for (VertexDepth i = 1; i < max_depth; i++) {
    float condition = 100 - i * (100 / (float)(max_depth - 1));
    while (m < graph.vertices_in_depth(i).size()) {
      while (n < graph.vertices_in_depth(i + 1).size()) {
        if (probability(condition)) {
          std::uniform_int_distribution<int> random_vertex_distribution(
              0, graph.vertices_count_in_depth(i + 1) - 1);

          VertexId source = graph.vertices_in_depth(i).at(m);
          VertexId destination;
          VertexId random_vertex;

          bool break_flag = false;
          do {
            if (created_by_vertex(graph, source, new_vertices_num) ==
                graph.vertices_count_in_depth(i + 1)) {
              break_flag = true;
              break;
            }
            std::random_device rd;
            std::mt19937 mt(rd());
            random_vertex = random_vertex_distribution(mt);
            destination = graph.vertices_in_depth(i + 1).at(random_vertex);
          } while (source == graph.source_of_vertex(destination));

          if (break_flag)
            break;
          EdgeId edge_id = graph.get_edges().size();
          graph.insert_edge(source, destination, edge_id, Edge::Color::Yellow);
          break;
        }
        n++;
      }
      n = 0;
      m++;
    }
    m = 0;
  }
}

void generate_red_edges(Graph& graph, const VertexDepth& max_depth) {
  float condition = 67;
  int m = 0, n = 0;
  for (VertexDepth i = 0; i < max_depth; i++) {
    while (m != graph.vertices_in_depth(i).size()) {
      while (n != graph.vertices_in_depth(i + 2).size()) {
        if (probability(condition)) {
          std::uniform_int_distribution<int> random_vertex_distribution(
              0, graph.vertices_count_in_depth(i + 2) - 1);
          std::random_device rd;
          std::mt19937 mt(rd());
          VertexId random_vertex = random_vertex_distribution(mt);
          VertexId source = graph.vertices_in_depth(i).at(m);
          VertexId destination =
              graph.vertices_in_depth(i + 2).at(random_vertex);
          EdgeId edge_id = graph.get_edges().size();
          graph.insert_edge(source, destination, edge_id, Edge::Color::Red);
          break;
        }
        n++;
      }
      n = 0;
      m++;
    }
    m = 0;
  }
}

const int created_by_vertex(const Graph& graph,
                            const VertexId& id,
                            const int& new_vertices_num) {
  if (id == 0)
    return new_vertices_num;
  int count = 0;
  for (const auto& vertex : graph.get_vertices()) {
    if (graph.source_of_vertex(vertex.id) == id)
      count++;
  }
  return count;
}

int main() {
  int max_depth = -1;
  int new_vertices_num = -1;

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

  string filename = "./temp/graph1.json";
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
