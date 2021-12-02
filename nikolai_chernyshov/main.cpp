#include <algorithm>
#include <cassert>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <iterator>
#include <memory>
#include <random>
#include <vector>

using VertexId = int;
using EdgeId = int;
constexpr double GREEN_EDGE_PROBA = 0.1;
constexpr double RED_EDGE_PROBA = 0.33;
constexpr int MIN_DEPTH = 0;
constexpr int MIN_NEW_VERTICES_NUM = 0;

bool get_random_boolean(double proba) {
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

  Edge(const VertexId& _vertex1_id,
       const VertexId& _vertex2_id,
       const EdgeId& _id,
       const Color& _color = Color::Gray)
      : id(_id),
        vertex1_id(_vertex1_id),
        vertex2_id(_vertex2_id),
        color(_color) {}
};

struct Vertex {
 public:
  const VertexId id;
  int depth = 0;

  explicit Vertex(const VertexId& _id) : id(_id) {}

  bool has_edge_id(const EdgeId& id) const {
    if (edge_ids_.empty())
      return false;
    if (std::find(edge_ids_.begin(), edge_ids_.end(), id) != edge_ids_.end())
      return true;
    return false;
  }

  void add_edge_id(const EdgeId& id) {
    assert(!has_edge_id(id) && "Edge id already exists");
    edge_ids_.push_back(id);
  }

  const std::vector<EdgeId>& get_edge_ids() const { return edge_ids_; }

 private:
  std::vector<EdgeId> edge_ids_;
};

class Graph {
 public:
  bool are_connected(const VertexId& vertex1_id,
                     const VertexId& vertex2_id) const {
    assert(has_vertex(vertex1_id));
    assert(has_vertex(vertex2_id));

    if (vertex1_id != vertex2_id) {
      for (const auto& edge1_id : get_vertex(vertex1_id).get_edge_ids())
        for (const auto& edge2_id : get_vertex(vertex2_id).get_edge_ids())
          if (edge1_id == edge2_id)
            return true;
    } else {
      for (const auto& edge_id : get_vertex(vertex1_id).get_edge_ids()) {
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

  Vertex& get_vertex(const VertexId& vertex_id) {
    const auto& const_this = *this;
    return const_cast<Vertex&>(const_this.get_vertex(vertex_id));
  }

  void add_edge(const VertexId& vertex1_id, const VertexId& vertex2_id) {
    assert(has_vertex(vertex1_id));
    assert(has_vertex(vertex2_id));
    assert(!are_connected(vertex1_id, vertex2_id));

    const auto less_vertex_id = std::min(vertex1_id, vertex2_id);
    const auto greater_vertex_id = std::max(vertex1_id, vertex2_id);

    const auto color = calculate_edge_color(less_vertex_id, greater_vertex_id);

    const auto& new_edge = edges_.emplace_back(
        less_vertex_id, greater_vertex_id, get_max_edge_id(), color);

    Vertex& vertex1 = get_vertex(less_vertex_id);
    vertex1.add_edge_id(new_edge.id);
    if (less_vertex_id != greater_vertex_id) {
      Vertex& vertex2 = get_vertex(greater_vertex_id);
      vertex2.add_edge_id(new_edge.id);
    }

    if (color == Edge::Color::Gray) {
      set_vertex_depth(greater_vertex_id, vertex1.depth + 1);
    }
  }

  const VertexId& add_vertex() {
    const auto& new_vertex = vertices_.emplace_back(get_max_vertex_id());
    if (depth_map_.size() == 0)
      depth_map_.push_back({new_vertex.id});
    else
      depth_map_[0].push_back(new_vertex.id);
    return new_vertex.id;
  }

  const std::vector<Edge>& get_edges() const { return edges_; }

  const std::vector<Vertex>& get_vertices() const { return vertices_; }

  const std::vector<VertexId>& get_vertex_ids_in_depth(int depth) const {
    return depth_map_[depth];
  }

  const std::vector<std::vector<VertexId>>& get_depth_map() const {
    return depth_map_;
  }

  const int get_depth() const { return depth_map_.size() - 1; }

 private:
  std::vector<Edge> edges_;
  std::vector<Vertex> vertices_;
  std::vector<std::vector<VertexId>> depth_map_;
  VertexId vertex_id_max_ = 0;
  EdgeId edge_id_max_ = 0;

  void set_vertex_depth(const VertexId& vertex_id, const int depth) {
    Vertex& vertex = get_vertex(vertex_id);
    vertex.depth = depth;

    while (depth_map_.size() <= depth) {
      depth_map_.push_back({});
    }
    depth_map_[depth].push_back(vertex_id);

    for (auto vertex_id_on_zero_depth = depth_map_[0].begin();
         vertex_id_on_zero_depth != depth_map_[0].end();
         vertex_id_on_zero_depth++) {
      if (*vertex_id_on_zero_depth == vertex.id) {
        depth_map_[0].erase(vertex_id_on_zero_depth);
        break;
      }
    }
  }

  const Edge::Color calculate_edge_color(const VertexId& vertex1_id,
                                         const VertexId& vertex2_id) const {
    if (vertex1_id == vertex2_id) {
      return Edge::Color::Green;
    }

    const auto& vertex1 = get_vertex(std::min(vertex1_id, vertex2_id));
    const auto& vertex2 = get_vertex(std::max(vertex1_id, vertex2_id));
    auto depth_difference = std::abs(vertex1.depth - vertex2.depth);

    if ((vertex1.get_edge_ids().size() == 0) ||
        (vertex2.get_edge_ids().size() == 0)) {
      return Edge::Color::Gray;
    } else if (depth_difference == 1) {
      return Edge::Color::Yellow;
    } else if (depth_difference == 2) {
      return Edge::Color::Red;
    } else {
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
    explicit Params(const int _depth = 0, const int _new_vertices_num = 0)
        : depth(_depth), new_vertices_num(_new_vertices_num) {}

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

    graph.add_vertex();
    for (int depth = 0; depth < params_.depth; depth++) {
      // создал копию, чтобы не итерироваться по изменяемому массиву
      // т.к. при создании серых граней у новых вершин пересчитывается глубина
      const auto vertex_ids = graph.get_vertex_ids_in_depth(depth);

      for (const auto& vertex_id : vertex_ids) {
        for (int generate_try_num = 0;
             generate_try_num < params_.new_vertices_num; generate_try_num++) {
          if (get_random_boolean(1 - (double)depth * proba_step)) {
            const auto new_vertex_id = graph.add_vertex();
            graph.add_edge(vertex_id, new_vertex_id);
          }
        }
      }
    }
  }

  void add_green_edges(Graph& graph) const {
    for (const auto& vertex : graph.get_vertices())
      if (get_random_boolean(GREEN_EDGE_PROBA))
        graph.add_edge(vertex.id, vertex.id);
  }

  void add_yellow_edges(Graph& graph) const {
    // поставил 2 вместо 1, потому что с глубины 0 желтая грань не может
    // начинаться иначе она будет идти к потомку корневой вершины, что
    // противоречит условию работало бы и в случае 1, но тогда производились бы
    // лишние вычисления
    if (graph.get_depth() < 2)
      return;

    double proba_step = 1.0 / (double)(graph.get_depth() - 1);
    const auto& depth_map = graph.get_depth_map();

    for (auto vertex_ids_in_depth = depth_map.begin();
         vertex_ids_in_depth != depth_map.end() - 1; vertex_ids_in_depth++) {
      for (const auto& vertex_id : *vertex_ids_in_depth) {
        std::vector<VertexId> unconnected_vertex_ids;
        for (const auto& vertex_id_in_next_depth : *(vertex_ids_in_depth + 1)) {
          if (!graph.are_connected(vertex_id, vertex_id_in_next_depth)) {
            unconnected_vertex_ids.push_back(vertex_id_in_next_depth);
          }
        }
        if (unconnected_vertex_ids.size() &&
            get_random_boolean(proba_step * (double)(vertex_ids_in_depth -
                                                     depth_map.begin()))) {
          graph.add_edge(vertex_id,
                         get_random_vertex_id(unconnected_vertex_ids));
        }
      }
    }
  };

  void add_red_edges(Graph& graph) const {
    if (graph.get_depth() < 2)
      return;
    for (auto vertex_ids_in_depth = graph.get_depth_map().begin();
         vertex_ids_in_depth != graph.get_depth_map().end() - 2;
         vertex_ids_in_depth++)
      for (const auto& vertex_id : *vertex_ids_in_depth)
        if (get_random_boolean(RED_EDGE_PROBA))
          graph.add_edge(vertex_id,
                         get_random_vertex_id(*(vertex_ids_in_depth + 2)));
  };

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
    res += std::to_string(vertex.depth);
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
    std::cout << "Invalid value, please input natural number or zero:"
              << std::endl;
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
    std::cout << "Invalid value, please input natural number or zero:"
              << std::endl;
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
