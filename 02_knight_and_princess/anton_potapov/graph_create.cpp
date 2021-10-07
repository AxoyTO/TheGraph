#include <cassert>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

using VertexId = int;
using EdgeId = int;

struct Vertex {
  VertexId id;
  Vertex(int vertex_id) : id(vertex_id) {}
};

struct Edge {
  EdgeId id;
  VertexId vertex1_id, vertex2_id;
  Edge(EdgeId e_id, VertexId ver1, VertexId ver2)
      : id(e_id), vertex1_id(ver1), vertex2_id(ver2) {}
};

class Graph {
 public:
  Graph() = default;
  Graph(const std::vector<Vertex>& vertices, const std::vector<Edge>& edges)
      : vertices_(vertices), edges_(edges) {
    for (const auto& edge : edges) {
      adjacency_list_[edge.vertex1_id].push_back(edge.id);
      adjacency_list_[edge.vertex2_id].push_back(edge.id);
    }
  }
  std::string get_json_string() const {
    std::stringstream json_stringstream;
    json_stringstream << "{\n";

    json_stringstream << "\t\"vertices\": [\n";
    for (auto it = adjacency_list_.begin(); it != adjacency_list_.end(); ++it) {
      const auto& [ith_vertex_id, ith_adjacency_list] = *it;
      json_stringstream << "\t\t{\n";
      json_stringstream << "\t\t\t\"id\": " << ith_vertex_id << ",\n";
      json_stringstream << "\t\t\t\"edge_ids\": [";
      for (size_t j = 0; j < ith_adjacency_list.size(); ++j) {
        json_stringstream << ith_adjacency_list[j];
        if (j + 1 != ith_adjacency_list.size()) {
          json_stringstream << ", ";
        }
      }
      json_stringstream << "]\n";
      json_stringstream << "\t\t}";
      if (std::next(it) == adjacency_list_.end()) {
        json_stringstream << "\n";
      } else {
        json_stringstream << ",\n";
      }
    }
    json_stringstream << "\t],";

    json_stringstream << " \"edges\": [\n";
    for (size_t i = 0; i < edges_.size(); ++i) {
      json_stringstream << "\t\t{\n";
      json_stringstream << "\t\t\t\"id\": " << edges_[i].id << ",\n";
      json_stringstream << "\t\t\t\"vertex_ids\": [" << edges_[i].vertex1_id
                        << ", " << edges_[i].vertex2_id << "]\n";
      json_stringstream << "\t\t}";
      if (i + 1 == edges_.size()) {
        json_stringstream << "\n";
      } else {
        json_stringstream << ",\n";
      }
    }
    json_stringstream << "\t]\n";

    json_stringstream << "}\n";
    return json_stringstream.str();
  }

 private:
  std::vector<Vertex> vertices_;
  std::vector<Edge> edges_;
  std::map<VertexId, std::vector<VertexId>> adjacency_list_;
};

Graph task_02_get_graph(void) {
  size_t n = 14;
  std::vector<Vertex> v_ans;
  for (size_t i = 0; i < n; ++i) {
    v_ans.push_back(Vertex(i));
  }
  std::vector<Edge> e_ans{Edge(0, 0, 1),    Edge(1, 0, 2),    Edge(2, 0, 3),
                          Edge(3, 1, 4),    Edge(4, 1, 5),    Edge(5, 1, 6),
                          Edge(6, 2, 7),    Edge(7, 2, 8),    Edge(8, 3, 9),
                          Edge(9, 4, 10),   Edge(10, 5, 10),  Edge(11, 6, 10),
                          Edge(12, 7, 11),  Edge(13, 8, 11),  Edge(14, 9, 12),
                          Edge(15, 10, 13), Edge(16, 11, 13), Edge(17, 12, 13)};
  return Graph(v_ans, e_ans);
}

int main() {
  Graph g = task_02_get_graph();
  std::string json_string = g.get_json_string();

  std::fstream json_file;
  json_file.open("graph.json", std::ios::out);
  assert(json_file && "file not created");
  json_file << json_string;
  json_file.close();
}
