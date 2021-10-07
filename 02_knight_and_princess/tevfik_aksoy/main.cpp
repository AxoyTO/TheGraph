#include <ctime>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <set>
#include <string>
#include <tuple>
#include <utility>
#include <vector>

using std::cout;
using std::endl;
using std::get;
using std::rand;
using std::set;
using std::string;
using std::vector;

static int st_depth = 0;
static int st_vertex_count = 0;
static int st_edge_val = 0;
static int st_condition = 0;
int creator_vertex = 0;
int new_vertex_flag = 0;
int max_depth;
int new_vertices_num;

using VertexId = int;
using EdgeId = int;
using VertexDepth = int;
using EdgeColor = string;

using DestID_EdgeID_SrcDepth_DestDepth_Color =
    std::tuple<VertexId,
               EdgeId,
               VertexDepth,
               VertexDepth,
               EdgeColor>;  // dest, edge_val, src_depth, dest_depth

struct Vertex {
  VertexId id;
  VertexDepth depth;

  Vertex() : id(0), depth(0) {}

  Vertex(int id_, int depth_) : id(id_), depth(depth_) {}

  std::string to_json(vector<int> edge_ids, bool end_flag) const {
    std::string str = "\t{ \"id\": " + std::to_string(id) + ", \"edge_ids\": [";
    for (int i = 0; i < edge_ids.size(); i++) {
      str += std::to_string(edge_ids[i]);
      if (i + 1 == edge_ids.size()) {
        str += "], \"depth\": ";
        str += std::to_string(depth);
        if (!end_flag)
          str += " },\n";
        else
          str += " }\n  ],\n";
      } else {
        str += ", ";
      }
    }
    return str;
  }
};

struct Edge {
 public:
  EdgeId id;
  EdgeColor color;
  VertexId source;
  VertexId destination;

  Edge() : id(0), source(), destination() {}

  Edge(VertexId _source,
       VertexId _destination,
       VertexId _id,
       EdgeColor _color = "")
      : id(_id), color(_color), source(_source), destination(_destination) {}

  std::string to_json(int end_flag) const {
    std::string str = "\t{ \"id\": " + std::to_string(id) +
                      ", \"vertex_ids\": [" + std::to_string(source) + ", " +
                      std::to_string(destination) + R"(], "color": ")" + color +
                      "\"";
    if (!end_flag)
      str += " },\n";
    else
      str += " }\n  ]\n}\n";
    return str;
  }
};

class Graph {
 public:
  vector<vector<DestID_EdgeID_SrcDepth_DestDepth_Color>> graph_data;
  vector<Edge> edges;
  vector<Vertex> vertices;

  Graph() {
    clear();
    generate_vertices();
    generate_edges();
  }

  // insert_node adds a new node and inserts a new simple(gray) edge to connect
  // the new node with the source
  void insert_node(int c_node,
                   int local_node_num,
                   int local_edge_value,
                   int src_depth,
                   int dest_depth,
                   const string& color);

  // insert_edge adds a new non-simple(non-gray) edge
  void insert_edge(Vertex src, Vertex dest, int edge_val, const string& color);

  // Recursive function to generate nodes using insert_node
  void generate_vertices();

  // Simple function that gathers other colored edge creating functions in it
  void generate_edges();

  // Function that generates(10%) green edges. A green edge is an edge from a
  // vertex which is source and destination itself
  void generate_green_edges();

  // Function that generates(25%) blue edges. A blue edge is an edge which
  // connects two neighbor vertices in the same depth
  void generate_blue_edges();

  // Function that generates yellow edges. A yellow edge is an edge which
  // connects a vertex with a deeper(by 1 level) vertex on variable probability
  void generate_yellow_edges();

  // Function that generates red edges. A red edge is an edge which connects a
  // vertex with a deeper(by 2 levels) vertex on 33% probability
  void generate_red_edges();

  // Checks if there is such an edge already
  bool there_is_edge(const Edge& edge);

  // Checks if there is such vertex as a source, if there is not, then
  // new_vertex_flag != 0
  bool there_is_vertex(int src_node_id);

  // Checks if there is such vertex as a destination, if there is not, then
  // dest_node_flag !=0;
  [[maybe_unused]] bool there_is_dest_vertex(int dest_node_id);

  // Returns vertex depth
  int depth_of(int vertex_id);

  // Returns total number of nodes
  int total_num_nodes() const;

  // Returns total number of edges
  unsigned long total_num_edges() const;

  // Returns total number of source nodes
  int total_num_src_nodes();

  // Returns total number of vertices in the depth
  unsigned long vertices_in_depth(int depth) const;

  // Returns color of the edge
  [[maybe_unused]] string color_of_edge(int edge_val);

  // Returns total number of edges of defined color
  int total_edges_of_color(const string& color) const;

  // Returns edge info
  [[maybe_unused]] Edge edge_info(int edge_val);

  // Returns true if the vertex is a source vertex
  [[maybe_unused]] bool is_source(int vertex_id);

  // Returns all the nodes with depth(x)
  vector<Vertex> nodes_with_depth(int n_d);

  // Display values in graph
  [[maybe_unused]] void displayGraphValues();

  // Resets and clears the values each time before creating a new graph
  static void clear();

  // Writes graph info to JSON file
  void to_JSON();

  // Display Graph on console
  [[maybe_unused]] void displayGraph(const Graph& graph);

 private:
  int probability_ = 0;
  int condition_ = 0;
};

void Graph::insert_node(int source,
                        int destination,
                        int edge_id,
                        int source_depth,
                        int destination_depth,
                        const string& color) {
  graph_data.resize(st_vertex_count * 2);
  graph_data[source].push_back(std::make_tuple(
      destination, edge_id, source_depth, destination_depth, color));
  if (color != "green") {
    graph_data[destination].push_back(std::make_tuple(
        source, edge_id, destination_depth, source_depth, color));
    vertices.push_back(Vertex(source, source_depth));
  }
  edges.push_back(Edge(source, destination, edge_id, color));
}

void Graph::insert_edge(Vertex src,
                        Vertex dest,
                        int edge_val,
                        const string& color) {
  if (!there_is_edge(Edge(src.id, dest.id,
                          edge_val)))  // if there is an edge connecting these
                                       // vertices already, no need to duplicate
  {
    if (src.id == dest.id) {
      if (!there_is_vertex(
              src.id))  // a green edge for a node that has just become a
                        // source(and a destination itself)
      {
        st_vertex_count++;
        insert_node(src.id, dest.id, st_edge_val++, src.depth, dest.depth,
                    color);
        src.id++;
      } else {
        insert_node(src.id, dest.id, st_edge_val++, src.depth, dest.depth,
                    color);
      }
    } else {
      st_vertex_count++;
      insert_node(src.id, dest.id, st_edge_val++, src.depth, dest.depth, color);
    }
  }
}

void Graph::generate_vertices() {
  if (st_condition <= 100) {
    unsigned long depth_node_count = vertices_in_depth(st_depth);
    for (size_t i = 0; i < depth_node_count; i++) {
      for (size_t j = 0; j < new_vertices_num; j++) {
        probability_ = rand() % 100;
        if (probability_ >= st_condition) {
          Vertex source(creator_vertex, st_depth);
          Vertex destination(st_vertex_count + 1, st_depth + 1);
          insert_edge(source, destination, st_edge_val, "gray");
        }
      }
      creator_vertex++;
    }
    st_condition += (100 / max_depth);
    st_depth++;
    generate_vertices();
  }
}

void Graph::generate_edges() {
  clock_t begin, end;
  double elapsed_time;
  std::cout << "Generating green edges...   ";
  begin = clock();
  generate_green_edges();
  end = clock();
  elapsed_time = (double)(end - begin) / CLOCKS_PER_SEC;
  std::cout << "Generated green edges. Elapsed Time: " << elapsed_time << "\n";
  std::cout << "Generating blue edges...   ";
  begin = clock();
  generate_blue_edges();
  end = clock();
  elapsed_time = (double)(end - begin) / CLOCKS_PER_SEC;
  std::cout << "Generated blue edges. Elapsed Time: " << elapsed_time << "\n";
  std::cout << "Generating yellow edges...   ";
  begin = clock();
  generate_yellow_edges();
  end = clock();
  elapsed_time = (double)(end - begin) / CLOCKS_PER_SEC;
  std::cout << "Generated yellow edges. Elapsed Time: " << elapsed_time << "\n";
  std::cout << "Generating red edges...   ";
  begin = clock();
  generate_red_edges();
  end = clock();
  elapsed_time = (double)(end - begin) / CLOCKS_PER_SEC;
  std::cout << "Generated red edges. Elapsed Time: " << elapsed_time << "\n\n";
}

void Graph::generate_green_edges() {
  condition_ = 90;  //Зеленая: 10% что у вершины будеть грань сама на себя.
  for (int i = 0; i < total_num_nodes(); i++) {
    probability_ = rand() % 100;
    if (probability_ >= condition_) {
      insert_edge(Vertex(i, depth_of(i)), Vertex(i, depth_of(i)), st_edge_val,
                  "green");
    }
  }
  creator_vertex = total_num_nodes();
}

void Graph::generate_blue_edges() {
  condition_ = 75;
  for (int i = 0; i < total_num_nodes(); i++) {
    if (depth_of(i) == depth_of(i + 1)) {
      probability_ = rand() % 100;
      if (probability_ > condition_) {
        insert_edge(Vertex(i, depth_of(i)), Vertex(i + 1, depth_of(i)),
                    st_edge_val, "blue");
      }
    }
  }
}

void Graph::generate_yellow_edges() {
  condition_ = 0;
  for (int i = total_num_nodes() - 1; i > 0; i--) {
    condition_ = (max_depth - depth_of(i) * (100 / max_depth));
    for (int j = i - 1; j > 0; j--) {
      if (depth_of(j) == depth_of(i) - 1) {
        probability_ = rand() % 100;
        if (probability_ > condition_) {
          insert_edge(Vertex(j, depth_of(j)), Vertex(i, depth_of(i)),
                      st_edge_val, "yellow");
        }
      } else if (depth_of(j) == depth_of(i) - 2)
        break;
    }
  }
}

void Graph::generate_red_edges() {
  condition_ = 67;  //Красная: 33% что вершина будет соединена с рандомной
                    //вершиной, находящейся на 2 уровня глубже.
  int m = 0, n = 0;
  for (int i = 0; i < max_depth; i++) {
    while (m != nodes_with_depth(i).size()) {
      while (n != nodes_with_depth(i + 2).size()) {
        probability_ = rand() % 100;
        if (probability_ > condition_)
          insert_edge(nodes_with_depth(i).at(m),
                      nodes_with_depth(i + 2).at(n++), st_edge_val, "red");
      }
      n = 0;
      m++;
    }
    m = 0;
  }
}

bool Graph::there_is_edge(const Edge& edge) {
  int src_node_id, dest_node_id;
  for (int i = 0; i < graph_data.size(); i++) {
    src_node_id = i;
    for (auto& j : graph_data[i]) {
      dest_node_id = get<0>(j);
      if ((src_node_id == edge.source) && (dest_node_id == edge.destination)) {
        return true;
      }
    }
  }
  return false;
}

bool Graph::there_is_vertex(int src_node_id) {
  new_vertex_flag = 1;
  for (size_t i = 0; i < graph_data.size(); i++) {
    if (src_node_id == i)
      new_vertex_flag = 0;
  }
  return new_vertex_flag;
}

[[maybe_unused]] bool Graph::there_is_dest_vertex(int dest_node_id) {
  bool ret_val = false;
  for (auto& i : graph_data) {
    for (auto& j : i) {
      if (dest_node_id == get<0>(j)) {
        ret_val = true;
        return ret_val;
      }
    }
  }
  return ret_val;
}

int Graph::depth_of(int vertex_id) {
  int ret_val = -1;
  if (vertex_id == 0) {
    return 0;
  }
  for (auto& i : graph_data) {
    for (auto& j : i) {
      if (get<0>(j) == vertex_id) {
        ret_val = get<3>(j);
        return ret_val;
      }
    }
  }
  return ret_val;
}

int Graph::total_num_nodes() const {
  int total_vertex_count;
  set<int> total_nodes_set;
  total_nodes_set.insert(0);
  for (const auto& i : graph_data) {
    if (i.empty())
      continue;
    for (const auto& j : i)
      total_nodes_set.insert(get<0>(j));
  }
  total_vertex_count = total_nodes_set.size();
  return total_vertex_count;
}

unsigned long Graph::total_num_edges() const {
  return edges.size();
}

int Graph::total_num_src_nodes() {
  int total_src_vertex_count;
  set<int> total_src_nodes_set;
  for (int i = 0; i < graph_data.size(); i++) {
    if (graph_data[i].empty())
      continue;
    total_src_nodes_set.insert(i);
  }
  total_src_vertex_count = total_src_nodes_set.size();
  return total_src_vertex_count;
}

unsigned long Graph::vertices_in_depth(int depth) const {
  if (depth == 0)
    return 1;
  set<int> depth_vertices_set;

  for (const auto& i : graph_data) {
    for (const auto& j : i) {
      if (get<3>(j) == depth)
        depth_vertices_set.insert(get<0>(j));
    }
  }

  return depth_vertices_set.size();
}

[[maybe_unused]] string Graph::color_of_edge(int edge_val) {
  for (auto& i : graph_data)
    for (auto& j : i) {
      if (get<1>(j) == edge_val)
        return get<4>(j);
    }

  return "No such edge_val";
}

int Graph::total_edges_of_color(const string& color) const {
  int ret_val = 0;
  for (const auto& i : graph_data) {
    for (const auto& j : i)
      if (get<4>(j) == color)
        ret_val++;
  }
  return ret_val / 2;
}

[[maybe_unused]] Edge Graph::edge_info(int edge_val) {
  Edge e;
  for (int i = 0; i < graph_data.size(); i++) {
    for (int j = 0; j < graph_data[i].size(); j++) {
      if (get<1>(graph_data[i][j]) == edge_val) {
        e.source = i;
        // e.source.depth = depth_of(i);
        e.destination = get<0>(graph_data[i][j]);
        // e.destination.depth = get<3>(graph_data[i][j]);
        e.id = get<1>(graph_data[i][j]);
        e.color = get<4>(graph_data[i][j]);
        return e;
      }
    }
  }
  return e;
}

[[maybe_unused]] bool Graph::is_source(int vertex_id) {
  bool ret_val = false;
  for (int i = 0; i < total_num_nodes(); i++) {
    if (i == vertex_id && !graph_data[i].empty()) {
      ret_val = true;
      return ret_val;
    }
  }
  return ret_val;
}

vector<Vertex> Graph::nodes_with_depth(int n_d) {
  vector<Vertex> nodes_vec;
  Vertex node;
  for (int i = 0; i < total_num_nodes(); i++) {
    if (depth_of(i) == n_d) {
      node.id = i;
      node.depth = n_d;
      nodes_vec.push_back(node);
    }
  }
  return nodes_vec;
}

[[maybe_unused]] void Graph::displayGraphValues() {
  cout << "st_condition = " << st_condition << endl;
  cout << "st_edge_val = " << st_edge_val << endl;
  cout << "st_vertex_count = " << st_vertex_count << endl;
  cout << "creator_vertex = " << creator_vertex << endl;
  cout << "st_depth = " << st_depth << endl;
  cout << "***************" << endl;
  cout << "Max Depth = " << max_depth << endl;
  cout << "New vertices num = " << new_vertices_num << endl;
  cout << "Total source vertices = " << total_num_src_nodes() << endl;
  cout << "Total vertices = " << total_num_nodes() << endl;
  cout << "Total edges = " << graph_data.size() << endl;
  cout << "Edges of color gray = " << total_edges_of_color("gray") << endl;
  cout << "Edge of color green = " << total_edges_of_color("green") << endl;
  cout << "Edge of color blue = " << total_edges_of_color("blue") << endl;
  cout << "Edge of color yellow = " << total_edges_of_color("yellow") << endl;
  cout << "Edge of color red = " << total_edges_of_color("red") << endl;
}

void Graph::clear() {
  st_condition = 0;
  st_depth = 0;
  st_vertex_count =
      0;            // st_vertex_count is the amount of vertices in the graph.
  st_edge_val = 0;  // st_edge_val is also the amount of edges in the graph.
  creator_vertex = 0;
  new_vertex_flag = 0;
}

void Graph::to_JSON() {
  static int k = 1;
  srand(time(nullptr));
  string filename = "./temp/graph";
  filename = filename + std::to_string(k++) + ".json";
  std::ofstream file(filename, std::ios::out);
  if (!file.is_open()) {
    std::cerr << "Error while opening the file " << filename << endl;
  } else {
    Vertex src;
    vector<int> edge_ids;
    bool end_flag = false;
    file << "{\n\"vertices\": [\n";
    for (int i = 0; i < total_num_nodes(); i++) {
      if (graph_data[i].empty())
        continue;
      src.id = i;
      src.depth = depth_of(i);
      for (auto& j : graph_data[i]) {
        edge_ids.push_back(get<1>(j));
      }
      if (i + 1 == total_num_nodes())
        end_flag = true;
      file << src.to_json(edge_ids, end_flag);
      edge_ids.clear();
    }

    end_flag = false;
    file << "\"edges\": [\n";
    for (int i = 0; i < edges.size(); i++) {
      if (i + 1 == edges.size())
        end_flag = true;
      file << edges[i].to_json(end_flag);
    }
  }
}

[[maybe_unused]] void Graph::displayGraph(const Graph& graph) {
  for (int i = 0; i < graph_data.size(); i++) {
    for (int j = 0; j < graph_data[i].size(); j++) {
      cout << "(" << i << ", " << get<0>(graph_data[i][j]) << ", "
           << get<1>(graph.graph_data[i][j]) << ", "
           << get<2>(graph.graph_data[i][j]) << ", "
           << get<3>(graph.graph_data[i][j]) << ")";
    }
    if (!graph_data[i].empty())
      cout << endl;
  }
}

int main() {
  string directory = "./temp";
  try {
    if (!std::filesystem::create_directory(directory)) {
    }
  } catch (const std::exception& e) {
    std::cerr << e.what() << endl;
  }

  cout << "Enter max_depth: ";
  std::cin >> max_depth;
  cout << "Enter new_vertices_num: ";
  std::cin >> new_vertices_num;
  srand(time(nullptr));

  Graph graphs[10];
  for (auto& graph : graphs) {
    graph.to_JSON();
  }
  return 0;
}
