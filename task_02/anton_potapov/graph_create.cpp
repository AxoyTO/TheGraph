#include <iostream>
#include <algorithm>
#include <vector>
#include <cstdlib>
#include <map>
#include <set>
#include <cassert>

#include <properties.h>

enum 
{
    ROAD_EDGE_CHANCE_FACTOR = 1,
    COMMOD_EDGE_CHANCE_FACTOR = 3
};

struct Vertex
{
    int id;
    Camp camp_info;
    Vertex(int v_id, c_info = Camp()) : id(v_id), camp_info(c_info) {}
};

struct Edge
{
    int id;
    int v1_id, v2_id;
    Boss boss_info;
    Edge(int e_id, int ver1, int ver2, b_info = Boss()) : id(e_id), v1_id(ver1), v2_id(ver2), boss_info(b_info) {}
};

void dfs_get_colors(std::vector<std::vector<int> > &adjacency_list, std::vector<int> &color, int cur_color, int cur_v)
{
    color[cur_v] = cur_color;
    for (auto el : adjacency_list[cur_v]) {
        if (!color[el]) {
            dfs_get_colors(adjacency_list, color, cur_color, el);
        }
    }
}

int get_rand_el(std::set<int> s)
{
    int id = rand() % s.size();
    auto it = s.begin();
    std::advance(it, id);
    return *it;
}

class Graph
{
public:
    Graph(int n, bool loops = False)
    {
        std::vector<std::pair<int, int> > e_ans;

        int roads = rand() % n + 1;
        for (int i = 0; i < roads; ++i) {
            std::vector<int> v_cur();
            v_cur.push_back(0);
            v_cur.push_back(n - 1);
            for (int i = 1; i < n - 1; ++i) {
                if (rand() & ROAD_EDGE_CHANCE_FACTOR) {
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
                if (rand() & COMMOD_EDGE_CHANCE_FACTOR) {
                    e_ans.push_back(std::make_pair(i, j));
                    if (i != j) {
                        e_ans.push_back(std::make_pair(j, i));
                    }
                }
            }
        }

        std::vector<std::vector<int> > adjacency_list(n, std::vector<int>(0));
        for (auto el : e_ans) {
            adjacency_list[el.first].push_back(el.second);
        }

        //this with help us make graph connected
        std::vector<int> color(n, 0);
        int cur_color = 1;
        for (int i = 0; i < n; ++i) {
            if (!color[i]) {
                dfs_get_colors(adjacency_list, color, cur_color, i);
                ++cur_color;
            }
        }
        std::map<int, std::set<int> > color_distribution;
        for (int i = 0; i < n; ++i) {
            color_distribution[color[i]].insert(i);
        }

        //merging all subgraphs into the first subgraph by choosing random vertexes in each one
        while (color_distribution.size() != 1) {
            auto it_first = color_distribution.begin();
            auto it_second = color_distribution.begin();
            ++it_second;

            int el1 = get_rand_el(it_first->second);
            int el2 = get_rand_el(it_second->second);
            e_ans.push_back(std::make_pair(el1, el2));
            adjacency_list[el1].push_back(el2);
            adjacency_list[el2].push_back(el1);

            it_first->second.insert(it_second->second.begin(), it_second->second.end());
            color_distribution.erase(it_second);
        }
        //now graph is connected

        std::vector<Vertex> v_ans;
        for (int i = 0; i < n; ++i) {
            v_ans.push_back(Vertex(v_id=i));
        }
        
        std::map<int, std::vector<int> > adjacency_ans;
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

        vertices = v_ans;
        edges = edges_ans;
        adjacency = adjacency_ans;
    }
private:
    std::vector<Vertex> vertices;
    std::vector<Edge> edges;
    std::map<int, std::vector<int> > adjacency;
};

int main()
{
    Graph g = Graph(n=20, loops=true)
}
