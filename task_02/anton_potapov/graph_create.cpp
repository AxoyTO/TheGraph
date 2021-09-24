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

enum { ROAD_EDGE_CHANCE_FACTOR = 1, COMMOD_EDGE_CHANCE_FACTOR = 7 };

struct Vertex {
  int id;
  Camp camp_info;
  Vertex(int v_id, Camp c_info = Camp()) : id(v_id), camp_info(c_info) {}
};

struct Edge {
  int id;
  int v1_id, v2_id;
  Boss boss_info;
  Edge(int e_id, int ver1, int ver2, Boss b_info = Boss())
      : id(e_id), v1_id(ver1), v2_id(ver2), boss_info(b_info) {}
};

void dfs_get_colors(std::vector<std::vector<int>> &adjacency_list,
                    std::vector<int> &color, int cur_color, int cur_v) {
  color[cur_v] = cur_color;
  for (auto el : adjacency_list[cur_v]) {
    if (!color[el]) {
      dfs_get_colors(adjacency_list, color, cur_color, el);
    }
  }
}

int get_rand_el(std::set<int> &s) {
  int id = rand() % s.size();
  auto it = s.begin();
  std::advance(it, id);
  return *it;
}

class Graph {
public:
  Graph(const std::string &s) {
    int n = 0;
    std::vector<Vertex> v_ans;
    std::vector<Edge> e_ans;
    std::vector<std::vector<int>> a_ans;
    if (s == "task_02") {
      n = 14;
      a_ans = std::vector<std::vector<int>>(n, std::vector<int>());
      for (int i = 0; i < n; ++i) {
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
      for (auto el : e_ans) {
        a_ans[el.v1_id].push_back(el.id);
        a_ans[el.v2_id].push_back(el.id);
      }
    }
    vertices_ = v_ans;
    edges_ = e_ans;
    adjacency_list_ = a_ans;
  }

  /* generation of random graph with n nodes */
  Graph(int n, bool loops = false) {
    std::vector<std::pair<int, int>> e_ans;

    int roads = rand() % n + 1;
    for (int i = 0; i < roads; ++i) {
      std::vector<int> v_cur;
      v_cur.push_back(0);
      v_cur.push_back(n - 1);
      for (int i = 1; i < n - 1; ++i) {
        if (!(rand() & ROAD_EDGE_CHANCE_FACTOR)) {
          v_cur.push_back(i);
        }
      }
      std::random_shuffle(v_cur.begin(), v_cur.end());
      for (size_t i = 1; i < v_cur.size(); ++i) {
        e_ans.push_back(std::make_pair(i - 1, i));
        e_ans.push_back(std::make_pair(i, i - 1));
      }
    }
    for (int i = 0; i < n; ++i) {
      for (int j = i; j < n; ++j) {
        if (i == j && !loops) {
          continue;
        }
        if (!(rand() & COMMOD_EDGE_CHANCE_FACTOR)) {
          e_ans.push_back(std::make_pair(i, j));
          if (i != j) {
            e_ans.push_back(std::make_pair(j, i));
          }
        }
      }
    }

    std::vector<std::vector<int>> adjacency_list(n, std::vector<int>());
    for (auto el : e_ans) {
      adjacency_list[el.first].push_back(el.second);
    }

    // this with help us make graph connected
    std::vector<int> color(n, 0);
    int cur_color = 1;
    for (int i = 0; i < n; ++i) {
      if (!color[i]) {
        dfs_get_colors(adjacency_list, color, cur_color, i);
        ++cur_color;
      }
    }

    std::map<int, std::set<int>> color_distribution;
    for (int i = 0; i < n; ++i) {
      color_distribution[color[i]].insert(i);
    }

    // merging all subgraphs into the first subgraph by choosing random vertexes
    // in each one
    while (color_distribution.size() != 1) {
      auto it_first = color_distribution.begin();
      auto it_second = color_distribution.begin();
      ++it_second;

      int el1 = get_rand_el(it_first->second);
      int el2 = get_rand_el(it_second->second);
      e_ans.push_back(std::make_pair(el1, el2));
      adjacency_list[el1].push_back(el2);
      adjacency_list[el2].push_back(el1);

      it_first->second.insert(it_second->second.begin(),
                              it_second->second.end());
      color_distribution.erase(it_second);
    }
    // now graph is connected

    std::vector<Vertex> v_ans;
    for (int i = 0; i < n; ++i) {
      v_ans.push_back(Vertex(i));
    }

    std::vector<std::vector<int>> adjacency_ans(n, std::vector<int>());
    std::vector<Edge> edges_ans;
    int cur_e_id = 1;
    for (auto el : e_ans) {
      if (el.first <= el.second) {
        auto cur_edge = Edge(cur_e_id, el.first, el.second);
        ++cur_e_id;
        edges_ans.push_back(cur_edge);
        adjacency_ans[el.first].push_back(cur_edge.id);
        adjacency_ans[el.second].push_back(cur_edge.id);
      }
    }

    vertices_ = v_ans;
    edges_ = edges_ans;
    adjacency_list_ = adjacency_ans;
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
      json_file << "\t\t\t\"vertex_ids\": [" << edges_[i].v1_id << ", "
                << edges_[i].v2_id << "]\n";
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
  std::vector<std::vector<int>> adjacency_list_;
};

int main() {
  Graph g("task_02");
  g.create_json("graph.json");
}
