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
static int st_node_count = 0;
static int st_edge_val = 0;
static int st_condition = 0;
int creator_node = 0;
int new_node_flag = 0;
int max_depth;
int new_vertices_num;

// Tuple of <int,int,int,int> for realization of vector of vectors of
// <int,int,int,int>
typedef std::tuple<int, int, int, int, string>
    DestID_EdgeID_SrcDepth_DestDepth;  // dest, edge_val, src_depth, dest_depth

// Structure of nodes(vertices)
struct Node {
 public:
  Node() {
    id_ = 0;
    depth_ = 0;
  }

  Node(int id, int depth) {
    this->id_ = id;
    this->depth_ = depth;
  }

  int GetID() const { return id_; }

  int GetDepth() const { return depth_; }

  void SetID(int val) { id_ = val; }

  void SetDepth(int val) { depth_ = val; }

  std::string to_json(vector<int> edge_ids, bool end_flag) const {
    std::string str =
        "\t{ \"id\": " + std::to_string(id_) + ", \"edge_ids\": [";
    for (int i = 0; i < edge_ids.size(); i++) {
      str += std::to_string(edge_ids[i]);
      if (i + 1 == edge_ids.size()) {
        str += "], \"depth\": ";
        str += std::to_string(depth_);
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

 private:
  int id_ = 0;
  int depth_ = 0;
};

// Structure of the edge including the source and destination nodes in it
struct Edge {
 public:
  Node src_node = {};
  Node dest_node = {};

  Edge() {
    Node tmpsrc;
    Node tmpdest;
    src_node = tmpsrc;
    dest_node = tmpdest;
    id_ = -1;
    color_ = "colorless";
  }

  Edge(Node src_node, Node dest_node, int edge_id) {
    this->src_node = src_node;
    this->dest_node = dest_node;
    id_ = edge_id;
  }

  void SetID(int val) { id_ = val; }

  void SetColor(string val) { color_ = std::move(val); }

  std::string to_json(int end_flag) const {
    std::string str = "\t{ \"id\": " + std::to_string(id_) +
                      ", \"vertex_ids\": [" + std::to_string(src_node.GetID()) +
                      ", " + std::to_string(dest_node.GetID()) +
                      R"(], "color": ")" + color_ + "\"";
    if (!end_flag)
      str += " },\n";
    else
      str += " }\n  ]\n}\n";
    return str;
  }

 private:
  int id_ = 0;
  string color_;
};

class Graph {
 public:
  Graph() {
    clear();
    generate_vertices();
    generate_edges();
  }

  vector<vector<DestID_EdgeID_SrcDepth_DestDepth>> GetGraphVec() const {
    return graphVec;
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
  void insert_edge(Node src, Node dest, int edge_val, const string& color);

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
  // new_node_flag != 0
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
  vector<Node> nodes_with_depth(int n_d);

  // Display values in graph
  [[maybe_unused]] void displayGraphValues();

  // Resets and clears the values each time before creating a new graph
  static void clear();

  // Writes graph info to JSON file
  void to_json();

  // Display Graph on console
  [[maybe_unused]] void displayGraph();

 private:
  int probability_ = 0;
  int condition_ = 0;
  vector<std::tuple<int, int, string>>
      edgeSrcDestColor;  // vector of pairs to keep data of source and
                         // destination vertices of an edge
  vector<vector<DestID_EdgeID_SrcDepth_DestDepth>> graphVec;
};

void Graph::insert_node(int c_node,
                        int local_node_num,
                        int local_edge_value,
                        int src_depth,
                        int dest_depth,
                        const string& color) {
  graphVec.resize(st_node_count * 2);
  graphVec[creator_node].push_back(std::make_tuple(
      local_node_num, local_edge_value, src_depth, dest_depth, color));
  graphVec[local_node_num].push_back(
      std::make_tuple(c_node, local_edge_value, dest_depth, src_depth, color));
  edgeSrcDestColor.emplace_back(c_node, local_node_num, color);
}

void Graph::insert_edge(Node src,
                        Node dest,
                        int edge_val,
                        const string& color) {
  Edge e(src, dest, edge_val);
  if (!there_is_edge(e))  // if there is an edge connecting these vertices
                          // already, no need to duplicate
  {
    if (src.GetID() == dest.GetID()) {
      if (!there_is_vertex(
              src.GetID()))  // a green edge for a node that has just become a
                             // source(and a destination itself)
      {
        st_node_count++;
        insert_node(src.GetID(), dest.GetID(), st_edge_val++, src.GetDepth(),
                    dest.GetDepth(), color);
        src.SetID(src.GetID() + 1);
      } else {
        insert_node(src.GetID(), dest.GetID(), st_edge_val++, src.GetDepth(),
                    dest.GetDepth(), color);
      }
    } else {
      st_node_count++;
      insert_node(src.GetID(), dest.GetID(), st_edge_val++, src.GetDepth(),
                  dest.GetDepth(), color);
    }
  }
}

void Graph::generate_vertices() {
  // int successful_attempts = 0; //successful attempts count how many nodes are
  // successfully created in current depth
  Node src, dest;
  if (st_condition <= 100) {
    unsigned long depth_node_count = vertices_in_depth(st_depth);

    for (size_t i = 0; i < depth_node_count; i++) {
      for (size_t j = 0; j < new_vertices_num; j++) {
        probability_ = rand() % 100;
        if (probability_ >= st_condition) {
          src.SetID(creator_node);
          src.SetDepth(st_depth);
          dest.SetID(st_node_count + 1);
          dest.SetDepth(st_depth + 1);
          insert_edge(src, dest, st_edge_val, "gray");
        }
      }
      creator_node++;
    }
    st_condition += (100 / max_depth);
    st_depth++;
    generate_vertices();
  }
}

void Graph::generate_edges() {
  // clock_t begin, end;
  // double elapsed_time;
  // std::cout << "Generating green edges...   ";
  // begin = clock();
  generate_green_edges();
  // end = clock();
  // elapsed_time = (double)(end - begin) / CLOCKS_PER_SEC;
  // std::cout << "Generated green edges. Elapsed Time: " << elapsed_time <<
  // "\n"; std::cout << "Generating blue edges...   "; begin = clock();
  generate_blue_edges();
  // end = clock();
  // elapsed_time = (double)(end - begin) / CLOCKS_PER_SEC;
  // std::cout << "Generated blue edges. Elapsed Time: " << elapsed_time <<
  // "\n";
  // std::cout << "Generating yellow edges...   ";
  // begin = clock();
  generate_yellow_edges();
  // end = clock();
  // elapsed_time = (double)(end - begin) / CLOCKS_PER_SEC;
  // std::cout << "Generated yellow edges. Elapsed Time: " << elapsed_time <<
  // "\n";
  // std::cout << "Generating red edges...   ";
  // begin = clock();
  generate_red_edges();
  // end = clock();
  // elapsed_time = (double)(end - begin) / CLOCKS_PER_SEC;
  // std::cout << "Generated red edges. Elapsed Time: " << elapsed_time <<
  // "\n\n";
}

void Graph::generate_green_edges() {
  condition_ = 90;  //Зеленая: 10% что у вершины будеть грань сама на себя.
  Node src;
  Node dest;
  for (int i = 0; i < total_num_nodes(); i++) {
    probability_ = rand() % 100;
    if (probability_ >= condition_) {
      src.SetID(i);
      dest.SetID(i);
      src.SetDepth(depth_of(src.GetID()));
      dest.SetDepth(depth_of(src.GetID()));
      insert_edge(src, dest, st_edge_val, "green");
    }
  }
  creator_node = total_num_nodes();
}

void Graph::generate_blue_edges() {
  condition_ = 75;  //Синяя: 25% что соседние вершины, находящиеся на одной
                    //глубине, будут соединены.
  int successful_attempt = 0;
  Node src, dest;
  for (size_t i = 0; i < graphVec.size(); i++) {
    if (graphVec[i].empty())
      continue;
    else {
      for (int j = graphVec[i].size() - 1; j >= 0; j--) {
        probability_ = rand() % 100;
        if (probability_ >= condition_) {
          if (j < graphVec[i].size() - 1) {
            if (get<3>(graphVec[i][j]) == get<3>(graphVec[i][j + 1])) {
              src.SetID(get<0>(graphVec[i][j]));
              src.SetDepth(get<3>(graphVec[i][j]));
              dest.SetID(get<0>(graphVec[i][j + 1]));
              dest.SetDepth(get<3>(graphVec[i][j + 1]));
              insert_edge(src, dest, st_edge_val, "blue");
              successful_attempt++;
            }
          }
          if ((j == graphVec[i].size() - 1) && (i != graphVec.size() - 1) &&
              (!graphVec[i + 1].empty())) {
            if (get<3>(graphVec[i][j]) == get<3>(graphVec[i + 1][0])) {
              src.SetID(get<0>(graphVec[i][j]));
              src.SetDepth(get<3>(graphVec[i][j]));
              dest.SetID(get<0>(graphVec[i + 1][0]));
              dest.SetDepth(get<3>(graphVec[i + 1][0]));
              insert_edge(src, dest, st_edge_val, "blue");
              successful_attempt++;
            }
          }
        }
      }
    }
  }
  for (int i = 0; i < graphVec.size(); i++) {
    if (graphVec[i].empty())
      continue;
    else {
      if (i < graphVec.size() - 1) {
        if (graphVec[i + 1].empty())
          continue;
        else {
          probability_ = rand() % 100;
          if (probability_ >= condition_) {
            if (get<2>(graphVec[i][0]) == get<2>(graphVec[i + 1][0])) {
              src.SetID(i);
              src.SetDepth(get<2>(graphVec[i][0]));
              dest.SetDepth(get<2>(graphVec[i][0]));
              dest.SetID(i + 1);
              insert_edge(src, dest, st_edge_val, "blue");
              successful_attempt++;
            }
            for (size_t j = 0; j < graphVec[i].size(); j++) {
              if (j < graphVec[i].size())
                if ((get<3>(graphVec[i][j]) == get<3>(graphVec[i + 1][0])) &&
                    (get<0>(graphVec[i][j]) ==
                     get<0>(graphVec[i + 1][0]) - 1)) {
                  src.SetID(get<0>(graphVec[i][j]));
                  src.SetDepth(get<3>(graphVec[i][j]));
                  dest.SetDepth(get<3>(graphVec[i][j]));
                  dest.SetID(get<0>(graphVec[i + 1][0]));
                  insert_edge(src, dest, st_edge_val, "blue");
                  successful_attempt++;
                }
            }
          }
        }
      }
    }
  }
  creator_node += successful_attempt;
}

void Graph::generate_yellow_edges() {
  condition_ = 0;
  Node src, dest;
  for (int i = total_num_nodes() - 1; i > 0; i--) {
    src.SetID(i);
    src.SetDepth(depth_of(i));
    condition_ = (max_depth - src.GetDepth()) * (100 / max_depth);
    for (int j = i - 1; j > 0; j--) {
      dest.SetID(j);
      dest.SetDepth(depth_of(j));
      if (dest.GetDepth() == src.GetDepth() - 1) {
        probability_ = rand() % 100;
        if (probability_ > condition_) {
          insert_edge(dest, src, st_edge_val, "yellow");
        }
      } else if (dest.GetDepth() == src.GetDepth() - 2)
        break;
    }
  }
}

void Graph::generate_red_edges() {
  condition_ = 67;  //Красная: 33% что вершина будет соединена с рандомной
                    //вершиной, находящейся на 2 уровня глубже.
  vector<Node> src_nodes, dest_nodes;

  int m = 0, n = 0, t;
  for (int i = 0; i < max_depth; i++) {
    src_nodes = nodes_with_depth(i);
    t = i + 2;
    dest_nodes = nodes_with_depth(t);

    while (m != src_nodes.size()) {
      while (n != dest_nodes.size()) {
        probability_ = rand() % 100;
        if (probability_ > condition_)
          insert_edge(src_nodes.at(m), dest_nodes.at(n++), st_edge_val, "red");
      }
      n = 0;
      m++;
    }
    m = 0;
  }
}

bool Graph::there_is_edge(const Edge& edge) {
  int src_node_id, dest_node_id;
  for (int i = 0; i < graphVec.size(); i++) {
    src_node_id = i;
    for (auto& j : graphVec[i]) {
      dest_node_id = get<0>(j);
      if ((src_node_id == edge.src_node.GetID()) &&
          (dest_node_id == edge.dest_node.GetID())) {
        return true;
      }
    }
  }
  return false;
}

bool Graph::there_is_vertex(int src_node_id) {
  new_node_flag = 1;
  for (size_t i = 0; i < graphVec.size(); i++) {
    if (src_node_id == i)
      new_node_flag = 0;
  }
  return new_node_flag;
}

[[maybe_unused]] bool Graph::there_is_dest_vertex(int dest_node_id) {
  bool ret_val = false;
  for (auto& i : graphVec) {
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
  for (auto& i : graphVec) {
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
  for (const auto& i : graphVec) {
    if (i.empty())
      continue;
    for (const auto& j : i)
      total_nodes_set.insert(get<0>(j));
  }
  total_vertex_count = total_nodes_set.size();
  return total_vertex_count;
}

unsigned long Graph::total_num_edges() const {
  return edgeSrcDestColor.size();
}

int Graph::total_num_src_nodes() {
  int total_src_vertex_count;
  set<int> total_src_nodes_set;
  for (int i = 0; i < graphVec.size(); i++) {
    if (graphVec[i].empty())
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

  for (const auto& i : graphVec) {
    for (const auto& j : i) {
      if (get<3>(j) == depth)
        depth_vertices_set.insert(get<0>(j));
    }
  }

  return depth_vertices_set.size();
}

[[maybe_unused]] string Graph::color_of_edge(int edge_val) {
  for (auto& i : graphVec)
    for (auto& j : i) {
      if (get<1>(j) == edge_val)
        return get<4>(j);
    }

  return "No such edge_val";
}

int Graph::total_edges_of_color(const string& color) const {
  int ret_val = 0;
  for (const auto& i : graphVec) {
    for (const auto& j : i)
      if (get<4>(j) == color)
        ret_val++;
  }
  return ret_val / 2;
}

[[maybe_unused]] Edge Graph::edge_info(int edge_val) {
  Edge e;
  for (int i = 0; i < graphVec.size(); i++) {
    for (int j = 0; j < graphVec[i].size(); j++) {
      if (get<1>(graphVec[i][j]) == edge_val) {
        e.src_node.SetID(i);
        e.src_node.SetDepth(depth_of(i));
        e.dest_node.SetID(get<0>(graphVec[i][j]));
        e.dest_node.SetDepth(get<3>(graphVec[i][j]));
        e.SetID(get<1>(graphVec[i][j]));
        e.SetColor(get<4>(graphVec[i][j]));
        return e;
      }
    }
  }
  return e;
}

[[maybe_unused]] bool Graph::is_source(int vertex_id) {
  bool ret_val = false;
  for (int i = 0; i < total_num_nodes(); i++) {
    if (i == vertex_id && !graphVec[i].empty()) {
      ret_val = true;
      return ret_val;
    }
  }
  return ret_val;
}

vector<Node> Graph::nodes_with_depth(int n_d) {
  vector<Node> nodes_vec;
  Node node;
  for (int i = 0; i < total_num_nodes(); i++) {
    if (depth_of(i) == n_d) {
      node.SetID(i);
      node.SetDepth(n_d);
      nodes_vec.push_back(node);
    }
  }
  return nodes_vec;
}

[[maybe_unused]] void Graph::displayGraphValues() {
  cout << "st_condition = " << st_condition << endl;
  cout << "st_edge_val = " << st_edge_val << endl;
  cout << "st_node_count = " << st_node_count << endl;
  cout << "creator_node = " << creator_node << endl;
  cout << "st_depth = " << st_depth << endl;
  cout << "***************" << endl;
  cout << "Max Depth = " << max_depth << endl;
  cout << "New vertices num = " << new_vertices_num << endl;
  cout << "Total source vertices = " << total_num_src_nodes() << endl;
  cout << "Total vertices = " << total_num_nodes() << endl;
  cout << "Total edges = " << graphVec.size() << endl;
  cout << "Edges of color gray = " << total_edges_of_color("gray") << endl;
  cout << "Edge of color green = " << total_edges_of_color("green") << endl;
  cout << "Edge of color blue = " << total_edges_of_color("blue") << endl;
  cout << "Edge of color yellow = " << total_edges_of_color("yellow") << endl;
  cout << "Edge of color red = " << total_edges_of_color("red") << endl;
}

void Graph::clear() {
  st_condition = 0;
  st_depth = 0;
  st_node_count = 0;  // st_node_count is the amount of vertices in the graph.
  st_edge_val = 0;    // st_edge_val is also the amount of edges in the graph.
  creator_node = 0;
  new_node_flag = 0;
}

void Graph::to_json() {
  static int k = 1;
  srand(time(nullptr));
  string filename = "./temp/graph";
  filename = filename + std::to_string(k++) + ".json";
  std::ofstream file(filename, std::ios::out);
  if (!file.is_open()) {
    std::cerr << "Error while opening the file " << filename << endl;
  } else {
    Node src;
    Edge e;
    vector<int> edge_ids;
    bool end_flag = false;
    file << "{\n\"vertices\": [\n";
    for (int i = 0; i < total_num_nodes(); i++) {
      if (graphVec[i].empty())
        continue;
      src.SetID(i);
      src.SetDepth(depth_of(i));
      for (auto& j : graphVec[i]) {
        edge_ids.push_back(get<1>(j));
      }
      if (i + 1 == total_num_nodes())
        end_flag = true;
      file << src.to_json(edge_ids, end_flag);
      edge_ids.clear();
    }

    end_flag = false;
    file << "\"edges\": [\n";
    for (int i = 0; i < edgeSrcDestColor.size(); i++) {
      if (i + 1 == edgeSrcDestColor.size())
        end_flag = true;

      e.SetID(i);
      e.src_node.SetID(get<0>(edgeSrcDestColor[i]));
      e.dest_node.SetID(get<1>(edgeSrcDestColor[i]));
      e.SetColor(get<2>(edgeSrcDestColor[i]));
      file << e.to_json(end_flag);
    }
  }
}

[[maybe_unused]] void Graph::displayGraph() {
  for (int i = 0; i < graphVec.size(); i++) {
    for (int j = 0; j < graphVec[i].size(); j++) {
      cout << "(" << i << ", " << get<0>(graphVec[i][j]) << ", "
           << get<1>(GetGraphVec()[i][j]) << ", " << get<2>(GetGraphVec()[i][j])
           << ", " << get<3>(GetGraphVec()[i][j]) << ")";
    }
    if (!graphVec[i].empty())
      cout << endl;
  }
}

void PrintandWrite(std::ostream& os, std::ostream& fs, const string& str) {
  os << str;
  fs << str;
}

struct Logger {
  string time_str;
  time_t now{};
  static int k;
  string logfilename = "./temp/logger.txt";

  void start() {
    std::ofstream logfile(logfilename, std::ios::app);
    if (!logfile.is_open())
      std::cerr << "Error while opening the file " << logfilename << endl;
    now = time(nullptr);
    time_str = ctime(&now);
    time_str.pop_back();
    PrintandWrite(
        cout, logfile,
        time_str + ": Graph " + std::to_string(k) + ", Generation Started\n");
  }

  void end(const Graph& graph) {
    std::ofstream logfile(logfilename, std::ios::app);
    if (!logfile.is_open())
      std::cerr << "Error while opening the file " << logfilename << endl;
    std::vector<std::pair<string, int>> colors = {
        {"gray", graph.total_edges_of_color("gray")},
        {"green", graph.total_edges_of_color("green")},
        {"blue", graph.total_edges_of_color("blue")},
        {"yellow", graph.total_edges_of_color("yellow")},
        {"red", graph.total_edges_of_color("red")}};

    now = time(nullptr);
    time_str = ctime(&now);
    time_str.pop_back();
    PrintandWrite(
        cout, logfile,
        time_str + ": Graph " + std::to_string(k++) + ", Generation Ended {\n");
    PrintandWrite(cout, logfile,
                  "  depth: " + std::to_string(max_depth) + ",\n");
    PrintandWrite(
        cout, logfile,
        "  new_vertices_num: " + std::to_string(new_vertices_num) + ",\n");
    PrintandWrite(
        cout, logfile,
        "  vertices: " + std::to_string(graph.total_num_nodes()) + ", [");

    for (int i = 0; i <= max_depth; i++) {
      PrintandWrite(cout, logfile,
                    "depth_" + std::to_string(i) + "_" +
                        std::to_string(graph.vertices_in_depth(i)));
      if (i != max_depth)
        PrintandWrite(cout, logfile, ", ");
    }
    PrintandWrite(cout, logfile, "],\n");
    PrintandWrite(
        cout, logfile,
        "  edges: " + std::to_string(graph.total_num_edges()) + ", {");
    for (int i = 0; i < colors.size(); i++) {
      PrintandWrite(cout, logfile,
                    colors[i].first + ": " + std::to_string(colors[i].second));
      if (i + 1 != colors.size())
        PrintandWrite(cout, logfile, ", ");
    }
    PrintandWrite(cout, logfile, "}\n}\n");
  }
};

int Logger::k = 1;

int main() {
  string path = "./temp";
  try {
    if (!std::filesystem::create_directory(path)) {
    }
  } catch (const std::exception& e) {
    std::cerr << e.what() << endl;
  }

  cout << "Enter max_depth: ";
  std::cin >> max_depth;
  cout << "Enter new_vertices_num: ";
  std::cin >> new_vertices_num;
  srand(time(nullptr));
  Logger log;


  Graph graphs[10];
  for (auto& graph : graphs) {
    log.start();
    graph.to_json();
    log.end(graph);
  }
  return 0;
}
