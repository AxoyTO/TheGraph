#include <algorithm>
#include <cassert>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <iterator>
#include <memory>
#include <vector>
#include <random>

using VertexId = int;
using EdgeId = int;
constexpr double GREEN_EDGE_PROBA = 0.1;
constexpr double RED_EDGE_PROBA = 0.33;
constexpr int VERTICES_COUNT = 14;
constexpr int MIN_DEPTH = 0;
constexpr int MIN_NEW_VERTICES_NUM = 0;

const bool get_random_boolean(const double& proba) {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::bernoulli_distribution random_boolean(proba);
  return random_boolean(gen);
}

const VertexId& get_random_vertex_id(const std::vector<VertexId>& vertex_ids) {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> random_vertex(0, vertex_ids.size() - 1);
  return vertex_ids[random_vertex(gen)];
}


struct Edge {
  enum class Color { Gray, Green, Yellow, Red };

  const EdgeId id;
  const VertexId vertex1_id, vertex2_id;
  const Color color;


  Edge(const VertexId& _vertex1_id, const VertexId& _vertex2_id, const EdgeId& _id, const Color& _color = Color::Gray)
      : id(_id), vertex1_id(_vertex1_id), vertex2_id(_vertex2_id), color(_color) {}
};

struct Vertex {
 public:
  const VertexId id;

  explicit Vertex(const VertexId& _id) : id(_id) {}

  bool has_edge_id(const EdgeId& id) const {
    if (edge_ids_.empty())
      return false;
    if (std::find(edge_ids_.begin(), edge_ids_.end(), id) != edge_ids_.end())
      return true;
    return false;
  }

  void set_depth(const int& _depth) {
    assert(depth_ == 0 && "Depth has already been set");
    depth_ = _depth;
  }

  void add_edge_id(const EdgeId& id) {
    assert(!has_edge_id(id) && "Edge id already exists");
    edge_ids_.push_back(id);
  }

  const std::vector<EdgeId>& get_edge_ids() const { return edge_ids_; }

  const int get_depth() const { return depth_; }

 private:
  int depth_ = 0;
  std::vector<EdgeId> edge_ids_;
};

class Graph {
 public:
  bool are_connected(const VertexId& vertex1_id,
                     const VertexId& vertex2_id) const {
    assert(has_vertex(vertex1_id));
    assert(has_vertex(vertex2_id));

    for (const auto& edge1_id : get_vertex(vertex1_id).get_edge_ids())
      for (const auto& edge2_id : get_vertex(vertex2_id).get_edge_ids())
        if (edge1_id == edge2_id) {
          if (vertex1_id != vertex2_id) {
            return true;
          }
          else for (const auto& edge_id : get_vertex(vertex1_id).get_edge_ids()) {
            const Edge& edge = get_edge(edge_id);
            if (edge.vertex1_id == edge.vertex2_id)
              return true;
          }
        }
    return false;
  }

  bool has_vertex(const VertexId& vertex_id) const {
    for (const auto& vertex : vertices_)
      if (vertex.id == vertex_id) 
        return true;
    return false;
  }

  const Edge& get_edge(const EdgeId& edge_id) const {
    for (const auto& edge : edges_) 
      if (edge.id == edge_id) 
        return edge;
    throw std::runtime_error("Edge with such id doesn't exist");
  }

  const Vertex& get_vertex(const VertexId& vertex_id) const {
    for (const auto& vertex : vertices_) 
      if (vertex.id == vertex_id) 
        return vertex;
    throw std::runtime_error("Vertex with such id doesn't exist");
  }

  void add_edge(const VertexId& vertex1_id, const VertexId& vertex2_id) {
    assert(has_vertex(vertex1_id));
    assert(has_vertex(vertex2_id));
    assert(!are_connected(vertex1_id, vertex2_id));

    const Edge::Color color = ([&]() {
      if (vertex1_id > vertex2_id) {
        return calculate_edge_color(vertex2_id, vertex1_id);
      }
      else {
        return calculate_edge_color(vertex1_id, vertex2_id);
      }
    })();

    const auto& new_edge =
        edges_.emplace_back(vertex1_id, vertex2_id, get_max_edge_id(), color);
    vertices_[vertex1_id].add_edge_id(new_edge.id);
    if (vertex1_id != vertex2_id) {
      vertices_[vertex2_id].add_edge_id(new_edge.id);
    }

    if (color == Edge::Color::Gray) {
      if (vertex1_id > vertex2_id) set_vertex_depth(vertex1_id, get_vertex(vertex2_id).get_depth() + 1);
      else set_vertex_depth(vertex2_id, get_vertex(vertex1_id).get_depth() + 1);
    }
  }

  void add_vertex() {
    vertices_.emplace_back(get_max_vertex_id());
    if (depth_map_.size() == 0) depth_map_.push_back({});
    depth_map_[0].push_back(vertices_.back().id);
  }

  void add_edge_to_required_graph(const VertexId& vertex1_id, const VertexId& vertex2_id) {
    assert(has_vertex(vertex1_id));
    assert(has_vertex(vertex2_id));
    assert(!are_connected(vertex1_id, vertex2_id));;

    const auto& new_edge =
        edges_.emplace_back(vertex1_id, vertex2_id, get_max_edge_id(), Edge::Color::Gray);
    vertices_[vertex1_id].add_edge_id(new_edge.id);
    if (vertex1_id != vertex2_id) {
      vertices_[vertex2_id].add_edge_id(new_edge.id);
    }
  }

  const std::vector<Edge>& get_edges() const { return edges_; }

  const std::vector<Vertex>& get_vertices() const { return vertices_; }

  const std::vector<VertexId>& get_vertices_in_depth(int depth) const { return depth_map_[depth]; }

  const std::vector<std::vector<VertexId>>& get_depth_map() const { return depth_map_; }

  const int get_depth() const { return depth_map_.size() - 1; }


 private:
  std::vector<Edge> edges_;
  std::vector<Vertex> vertices_;
  std::vector<std::vector<VertexId>> depth_map_;
  VertexId vertex_id_max_ = 0;
  EdgeId edge_id_max_ = 0;

  void set_vertex_depth(const VertexId& vertex_id, const int& depth) {
    Vertex& vertex = vertices_[vertex_id];
    vertex.set_depth(depth);

    if (depth_map_.size() <= depth) {
      depth_map_.push_back({});
    }
    depth_map_[depth].push_back(vertex_id);
    for (auto vertex_id_on_zero_depth = depth_map_[0].begin();
         vertex_id_on_zero_depth != depth_map_[0].end(); vertex_id_on_zero_depth++) {
      if (*vertex_id_on_zero_depth == vertex.id) {
        depth_map_[0].erase(vertex_id_on_zero_depth);
        break;
      }
    }
  }

  const Edge::Color calculate_edge_color(const VertexId& vertex1_id, const VertexId& vertex2_id) const {
    if (vertex1_id == vertex2_id) {
      return Edge::Color::Green;
    }

    const auto& vertex1 = get_vertex(vertex1_id);
    const auto& vertex2 = get_vertex(vertex2_id);
    auto depth_difference = vertex1.get_depth() - vertex2.get_depth();
    if (depth_difference < 0) depth_difference *= -1;

    if (vertex2.get_edge_ids().size() == 0) {
      return Edge::Color::Gray;
    }
    else if (depth_difference == 1) {
      return Edge::Color::Yellow;
    }
    else if (depth_difference == 2) {
      return Edge::Color::Red;
    }
    else {
      throw std::runtime_error("Can't calculate edge color");
    }
  }

  const EdgeId get_max_edge_id() {
    const auto id = edge_id_max_;
    edge_id_max_++;
    return id;
  }

  const VertexId get_max_vertex_id() {
    const auto id = vertex_id_max_;
    vertex_id_max_++;
    return id;
  }
};

class GraphGenerator {
 public:
  struct Params {
    explicit Params(const int _depth = 0, const int _new_vertices_num = 0) :
      depth(_depth), new_vertices_num(_new_vertices_num) {}

    const int depth = 0;
    const int new_vertices_num = 0;
  };

  explicit GraphGenerator(const Params& params = Params()) : params_(params) {}
  
  Graph generate() const {
    Graph graph;

    add_gray_edges(graph);
    add_green_edges(graph);
    add_yellow_edges(graph);
    add_red_edges(graph);

    return graph;
  }

  void add_gray_edges(Graph& graph) const {
    double proba_step = 1.0 / (double)params_.depth;
    VertexId current_vertex_num = 0, added_vertex_num = 1;

    graph.add_vertex();
    for (int depth = 0; depth < params_.depth; depth++) {
      for (int vertex_num = 0; vertex_num < graph.get_vertices_in_depth(depth).size(); vertex_num++) {
        for (int generate_try_num = 0; generate_try_num < params_.new_vertices_num;
            generate_try_num++) {
          if (get_random_boolean(1 - (double)depth * proba_step)) {
            graph.add_vertex();
            graph.add_edge(current_vertex_num, added_vertex_num);
            added_vertex_num++;
          }
        }
        current_vertex_num++;
      }
    }
  }

  void add_green_edges(Graph& graph) const {
    for (const auto& vertex : graph.get_vertices())
      if (get_random_boolean(GREEN_EDGE_PROBA))
        graph.add_edge(vertex.id, vertex.id);
  }

  void add_yellow_edges(Graph& graph) const {
    if (graph.get_depth() < 2) return;
    double proba_step = 1.0 / (double)(graph.get_depth() - 1);
    for (auto depth = (graph.get_depth_map()).begin();
        depth != graph.get_depth_map().end() - 1; depth++) {
      for (const auto& vertex_id : *depth) {
        std::vector<VertexId> vertex_ids_on_next_depth;
        for (const auto& vertex_id_on_next_depth : *(depth + 1)) {
          if (!graph.are_connected(vertex_id, vertex_id_on_next_depth)) {
            vertex_ids_on_next_depth.push_back(vertex_id_on_next_depth);
          }
        }
        if (vertex_ids_on_next_depth.size() && get_random_boolean(proba_step * (double)(depth - graph.get_depth_map().begin()))) {
          graph.add_edge(vertex_id, get_random_vertex_id(vertex_ids_on_next_depth));
        }
      }
    }
  };

  void add_red_edges(Graph& graph) const {
    if (graph.get_depth() < 2) return;
    for (auto depth = graph.get_depth_map().begin(); depth != graph.get_depth_map().end() - 2; depth++)
      for (const auto& vertex_id : *depth)
        if (get_random_boolean(RED_EDGE_PROBA))
          graph.add_edge(vertex_id, get_random_vertex_id(*(depth + 2)));
  };

  Graph build_required_graph() const {
    Graph graph;

    for (int i = 0; i < VERTICES_COUNT; i++) {
      graph.add_vertex();
    }

    graph.add_edge_to_required_graph(0, 1);
    graph.add_edge_to_required_graph(0, 2);
    graph.add_edge_to_required_graph(0, 3);
    graph.add_edge_to_required_graph(1, 4);
    graph.add_edge_to_required_graph(1, 5);
    graph.add_edge_to_required_graph(1, 6);
    graph.add_edge_to_required_graph(2, 7);
    graph.add_edge_to_required_graph(2, 8);
    graph.add_edge_to_required_graph(3, 9);
    graph.add_edge_to_required_graph(4, 10);
    graph.add_edge_to_required_graph(5, 10);
    graph.add_edge_to_required_graph(6, 10);
    graph.add_edge_to_required_graph(7, 11);
    graph.add_edge_to_required_graph(8, 11);
    graph.add_edge_to_required_graph(9, 12);
    graph.add_edge_to_required_graph(10, 13);
    graph.add_edge_to_required_graph(11, 13);
    graph.add_edge_to_required_graph(12, 13);

    return graph;
  }

 private:
  const Params params_ = Params();
};

class GraphPrinter {
 public:
  std::string vertex_to_json(const Vertex& vertex) const {
    std::string res;
    res += "{\n\t\t\t\"id\": ";
    res += std::to_string(vertex.id);
    res += ",\n\t\t\t\"edge_ids\": [";

    auto edge_ids = vertex.get_edge_ids();

    if (!edge_ids.empty()) {
      for (const auto& edge_id : edge_ids) {
        res += std::to_string(edge_id);
        res += ", ";
      }
      res.pop_back();
      res.pop_back();
    }
    res += "],\n\t\t\t\"depth\": ";
    res += std::to_string(vertex.get_depth());
    res += "\n\t\t}";

    return res;
  }

  std::string edge_to_json(const Edge& edge) const {
    std::string res;

    res += "{\n\t\t\t\"id\": ";
    res += std::to_string(edge.id);
    res += ",\n\t\t\t\"vertex_ids\": [";
    res += std::to_string(edge.vertex1_id);
    res += ", ";
    res += std::to_string(edge.vertex2_id);
    res += "],\n\t\t\t\"color\": \"";
    res += color_to_string(edge.color);
    res += "\"\n\t\t}, ";

    return res;
  }

  std::string color_to_string(const Edge::Color& color) const {
    switch (color) {
      case Edge::Color::Gray:
        return "gray";
      case Edge::Color::Green:
        return "green";
      case Edge::Color::Yellow:
        return "yellow";
      case Edge::Color::Red:
        return "red";
    }
    throw std::runtime_error("Invalid color value");
  }

  std::string to_json(const Graph& graph) const {
    std::string res;
    res += "{\n\t\"depth\": ";
    res += std::to_string(graph.get_depth());
    res += ",\n\t\"vertices\": [\n\t\t";

    auto vertices = graph.get_vertices();

    if (!vertices.empty()) {
      for (const auto& vertex : graph.get_vertices()) {
        res += vertex_to_json(vertex);
        res += ", ";
      }
      res.pop_back();
      res.pop_back();
    }

    res += "\n\t],\n\t\"edges\": [\n\t\t";

    auto edges = graph.get_edges();

    if (!edges.empty()) {
      for (const auto& edge : graph.get_edges()) {
        res += edge_to_json(edge);
      }
      res.pop_back();
      res.pop_back();
    }

    res += "\n\t]\n}\n";

    return res;
  }
};

const int handle_depth_input() {
  int depth;

  std::cout << "Input depth:" << std::endl;
  while (!(std::cin >> depth) || (depth < MIN_DEPTH)) {
    std::cout << "Invalid value, please input natural number or zero:" << std::endl;
    if (!std::cin) {
      std::cin.clear();
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
  }
  return depth;
}

const int handle_new_vertices_num_input() {
  int vertices_num;
  std::cout << "Input vertices num:" << std::endl;
  while (!(std::cin >> vertices_num) || (vertices_num < MIN_NEW_VERTICES_NUM)) {
    std::cout << "Invalid value, please input natural number or zero:" << std::endl;
    if (!std::cin) {
      std::cin.clear();
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
  }
  return vertices_num;
}

int main() {
  const int depth = handle_depth_input();
  const int new_vertices_num = handle_new_vertices_num_input();

  const auto params = GraphGenerator::Params(depth, new_vertices_num);
  const auto generator = GraphGenerator(params);
  const auto graph = generator.generate();

  const GraphPrinter printer;
  std::ofstream myfile;
  myfile.open("graph.json");
  myfile << printer.to_json(graph);
  myfile.close();

  return 0;
}
