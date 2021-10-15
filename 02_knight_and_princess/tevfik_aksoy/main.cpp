#include <cassert>
#include <ctime>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <random>
#include <string>
#include <vector>

std::random_device rd;
std::mt19937 mt(rd());
std::uniform_real_distribution<float> probability(0.0, 100.0);

using std::string;
using std::vector;

int max_depth;
int new_vertices_num;

using VertexId = int;
using EdgeId = int;
using VertexDepth = int;
using EdgeColor = string;

struct Vertex {
  const VertexId source_id;
  const VertexId id;
  const VertexDepth depth;

  Vertex(const VertexId& source_id_,
         const VertexId& id_,
         const VertexDepth& depth_)
      : source_id(source_id_), id(id_), depth(depth_) {}

  void add_edge(const EdgeId& _id, const EdgeColor& _color) {
    edge_ids_.push_back(_id);
    edge_colors_.push_back(_color);
  }

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
  vector<EdgeColor> edge_colors_;
};

struct Edge {
 public:
  const EdgeId id;
  const EdgeColor color;
  const VertexId source;
  const VertexId destination;

  Edge(VertexId _source,
       VertexId _destination,
       VertexId _id,
       EdgeColor _color = "")
      : id(_id), color(_color), source(_source), destination(_destination) {}

  std::string to_JSON() const {
    std::string json_string;
    json_string += "\t{ \"id\": " + std::to_string(id) + ", \"vertex_ids\": [" +
                   std::to_string(source) + ", " + std::to_string(destination) +
                   "], \"color\": \"" + color + "\" }";
    return json_string;
  }
};

class Graph {
 public:
  VertexId source_of_vertex(const VertexId& vertex) {
    return vertices_[vertex].source_id;
  }

  void insert_vertex(const VertexId& source_vertex,
                     const VertexId& vertex,
                     const VertexDepth& depth) {
    vertices_.emplace_back(source_vertex, vertex, depth);
  }
  void insert_edge(const VertexId& source,
                   const VertexId& destination,
                   const EdgeId& id,
                   const EdgeColor& color) {
    edges_.emplace_back(source, destination, id, color);
    if (color != "green") {
      vertices_[source].add_edge(id, color);
      vertices_[destination].add_edge(id, color);
    } else {
      vertices_[source].add_edge(id, color);
    }
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

  // Returns depth of vertex in the graph
  const VertexDepth depth_of(const VertexId& id) const {
    return vertices_[id].depth;
  }

  // Returns the number of vertices created by vertex in the graph
  const int created_by_vertex(const VertexId& id) const {
    if (id == 0)
      return new_vertices_num;
    int count = 0;
    for (const auto& vertex : vertices_) {
      if (vertex.source_id == id)
        count++;
    }
    return count;
  }

  vector<Vertex> get_vertices() const { return vertices_; }
  vector<Edge> get_edges() const { return edges_; }

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
    for (int i = 0; i < vertices_.size(); i++) {
      if (depth_of(i) == depth) {
        vertices.push_back(i);
      }
    }
    return vertices;
  }

 private:
  vector<Edge> edges_;
  vector<Vertex> vertices_;
};

void generate_vertices_and_gray_edges(Graph& graph);
void generate_green_edges(Graph& graph);
void generate_blue_edges(Graph& graph);
void generate_yellow_edges(Graph& graph);
void generate_red_edges(Graph& graph);

const Graph generateGraph() {
  Graph graph;
  clock_t begin, end;
  double elapsed_time;
  begin = clock();
  std::cout << "Generating vertices and gray edges...   ";
  generate_vertices_and_gray_edges(graph);
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
  generate_yellow_edges(graph);
  end = clock();
  elapsed_time = (double)(end - begin) / CLOCKS_PER_SEC;
  std::cout << "Generated yellow edges. Elapsed Time: " << elapsed_time
            << " s.\n";

  std::cout << "Generating red edges...   ";
  begin = clock();
  generate_red_edges(graph);
  end = clock();
  elapsed_time = (double)(end - begin) / CLOCKS_PER_SEC;
  std::cout << "Generated red edges. Elapsed Time: " << elapsed_time
            << " s.\n\n";

  return graph;
}

void generate_vertices_and_gray_edges(Graph& graph) {
  graph.insert_vertex(0, 0, 0);
  float condition = 0;
  VertexId source_vertex = 0;
  VertexId vertex = 1;

  for (VertexDepth depth = 0; depth < max_depth; depth++) {
    for (int i = 0; i < graph.vertices_count_in_depth(depth); i++) {
      for (int j = 0; j < new_vertices_num; j++) {
        if (probability(mt) >= condition) {
          graph.insert_vertex(source_vertex, vertex, depth + 1);
          EdgeId edge_id = graph.get_edges().size();
          graph.insert_edge(source_vertex, vertex, edge_id, "gray");
          vertex++;
        }
      }
      source_vertex++;
    }
    condition += 100 / (float)(max_depth);
  }
  if (max_depth != graph.depth_of(vertex - 1)) {
    std::cout << "\nMax depth couldn't be reached. Depth of final vertex: "
              << graph.depth_of(vertex - 1) << "\n";
    max_depth = graph.depth_of(vertex - 1);
  }
}

void generate_green_edges(Graph& graph) {
  int condition = 90;  //Зеленая: 10% что у вершины будеть грань сама на себя.
  for (const auto& vertex : graph.get_vertices()) {
    if (probability(mt) >= condition) {
      EdgeId edge_id = graph.get_edges().size();
      graph.insert_edge(vertex.source_id, vertex.source_id, edge_id, "green");
    }
  }
}

void generate_blue_edges(Graph& graph) {
  float condition = 75;
  for (int i = 0; i < graph.get_vertices().size() - 1; i++) {
    if (graph.depth_of(i) == graph.depth_of(i + 1))
      if (probability(mt) >= condition) {
        EdgeId edge_id = graph.get_edges().size();
        graph.insert_edge(i, i + 1, edge_id, "blue");
      }
  }
}

void generate_yellow_edges(Graph& graph) {
  int m = 0, n = 0;
  for (VertexDepth i = 1; i < max_depth; i++) {
    float condition = 100 - i * (100 / (float)(max_depth - 1));
    while (m < graph.vertices_in_depth(i).size()) {
      while (n < graph.vertices_in_depth(i + 1).size()) {
        if (probability(mt) >= condition) {
          std::uniform_int_distribution<int> random_vertex_distribution(
              0, graph.vertices_count_in_depth(i + 1) - 1);

          VertexId source = graph.vertices_in_depth(i).at(m);
          VertexId destination;
          VertexId random_vertex;

          bool break_flag = false;
          do {
            if (graph.created_by_vertex(source) ==
                graph.vertices_count_in_depth(i + 1)) {
              break_flag = true;
              break;
            }
            random_vertex = random_vertex_distribution(mt);
            destination = graph.vertices_in_depth(i + 1).at(random_vertex);
          } while (source == graph.source_of_vertex(destination));

          if (break_flag)
            break;
          EdgeId edge_id = graph.get_edges().size();
          graph.insert_edge(source, destination, edge_id, "yellow");
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

void generate_red_edges(Graph& graph) {
  float condition = 67;
  int m = 0, n = 0;
  for (VertexDepth i = 0; i < max_depth; i++) {
    while (m != graph.vertices_in_depth(i).size()) {
      while (n != graph.vertices_in_depth(i + 2).size()) {
        if (probability(mt) >= condition) {
          std::uniform_int_distribution<int> random_vertex_distribution(
              0, graph.vertices_count_in_depth(i + 2) - 1);
          VertexId random_vertex = random_vertex_distribution(mt);
          VertexId source = graph.vertices_in_depth(i).at(m);
          VertexId destination =
              graph.vertices_in_depth(i + 2).at(random_vertex);
          EdgeId edge_id = graph.get_edges().size();
          graph.insert_edge(source, destination, edge_id, "red");
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

int main() {
  string directory = "./temp";
  try {
    if (!std::filesystem::create_directory(directory)) {
    }
  } catch (const std::exception& ex) {
    std::cerr << ex.what() << "\n";
  }

  int graph_count;

  std::cout << "Enter max_depth: ";
  std::cin >> max_depth;
  std::cout << "Enter new_vertices_num: ";
  std::cin >> new_vertices_num;
  std::cout << "Enter the number of graphs to be created: ";
  std::cin >> graph_count;
  std::cout << "\n";

  int k = 1;
  for (int i = 0; i < graph_count; i++) {
    string filename = "./temp/graph";
    filename = filename + std::to_string(k++) + ".json";
    std::ofstream file(filename, std::ios::out);
    if (!file.is_open())
      std::cerr << "Error opening the file graph.json!\n";
    else {
      const auto graph = generateGraph();
      file << graph.to_JSON();
      file.close();
    }
  }
  return 0;
}