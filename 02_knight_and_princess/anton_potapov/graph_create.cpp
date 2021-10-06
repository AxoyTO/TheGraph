#include <algorithm>
#include <cassert>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <vector>

#include "properties.hpp"

using VertexId = int;
using EdgeId = int;

struct Vertex {
  VertexId id;
  Camp camp_info;
  Vertex(int vertex_id, Camp c_info = Camp()) : id(vertex_id), camp_info(c_info) {}
};

struct Edge {
  EdgeId id;
  VertexId vertex1_id, vertex2_id;
  Boss boss_info;
  Edge(EdgeId e_id, VertexId ver1, VertexId ver2, Boss b_info = Boss())
      : id(e_id), vertex1_id(ver1), vertex2_id(ver2), boss_info(b_info) {}
};

class Graph {
public:
  Graph(const std::string &s) {
    size_t n = 0;
    std::vector<Vertex> v_ans;
    std::vector<Edge> e_ans;
    std::vector<std::vector<VertexId>> a_ans;
    if (s == "task_02") {
      n = 14;
      a_ans = std::vector<std::vector<VertexId>>(n, std::vector<VertexId>());
      for (size_t i = 0; i < n; ++i) {
        v_ans.push_back(Vertex(i));
      }
      e_ans.push_back(Edge(0, 0, 1));
      e_ans.push_back(Edge(1, 0, 2));
      e_ans.push_back(Edge(2, 0, 3));
      e_ans.push_back(Edge(3, 1, 4));
      e_ans.push_back(Edge(4, 1, 5));
      e_ans.push_back(Edge(5, 1, 6));
      e_ans.push_back(Edge(6, 2, 7));
      e_ans.push_back(Edge(7, 2, 8));
      e_ans.push_back(Edge(8, 3, 9));
      e_ans.push_back(Edge(9, 4, 10));
      e_ans.push_back(Edge(10, 5, 10));
      e_ans.push_back(Edge(11, 6, 10));
      e_ans.push_back(Edge(12, 7, 11));
      e_ans.push_back(Edge(13, 8, 11));
      e_ans.push_back(Edge(14, 9, 12));
      e_ans.push_back(Edge(15, 10, 13));
      e_ans.push_back(Edge(16, 11, 13));
      e_ans.push_back(Edge(17, 12, 13));
      for (const auto &el : e_ans) {
        a_ans[el.v1_id].push_back(el.id);
        a_ans[el.v2_id].push_back(el.id);
      }
    }
    vertices_ = v_ans;
    edges_ = e_ans;
    adjacency_list_ = a_ans;
  }

  void create_json(const std::string &file_name) {
    std::fstream json_file;
    json_file.open(file_name, std::ios::out);
    assert(json_file && "file not created");
    json_file << "{\n";

    json_file << "\t\"vertices\": [\n";
    for (size_t i = 0; i < adjacency_list_.size(); ++i) {
      json_file << "\t\t{\n";
      json_file << "\t\t\t\"id\": " << i << ",\n";
      json_file << "\t\t\t\"edge_ids\": [";
      for (size_t j = 0; j < adjacency_list_[i].size(); ++j) {
        json_file << adjacency_list_[i][j];
        if (j != adjacency_list_[i].size() - 1) {
          json_file << ", ";
        }
      }
      json_file << "]\n";
      json_file << "\t\t}";
      if (i == adjacency_list_.size() - 1) {
        json_file << "\n";
      } else {
        json_file << ",\n";
      }
    }
    json_file << "\t],";

    json_file << " \"edges\": [\n";
    for (size_t i = 0; i < edges_.size(); ++i) {
      json_file << "\t\t{\n";
      json_file << "\t\t\t\"id\": " << edges_[i].id << ",\n";
      json_file << "\t\t\t\"vertex_ids\": [" << edges_[i].vertex1_id << ", "
                << edges_[i].vertex2_id << "]\n";
      json_file << "\t\t}";
      if (i == edges_.size() - 1) {
        json_file << "\n";
      } else {
        json_file << ",\n";
      }
    }
    json_file << "\t]\n";

    json_file << "}\n";
    json_file.close();
  }

private:
  std::vector<Vertex> vertices_;
  std::vector<Edge> edges_;
  std::vector<std::vector<VertexId>> adjacency_list_;
};

int main() {
  Graph g("task_02");
  g.create_json("graph.json");
}
