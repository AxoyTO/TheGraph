#include <iostream>
#include <fstream>
#include <vector>
#include <utility>

const int LEN = 14;

using std::pair;
using std::vector;
using std::cout;

class Graph {
public:
	struct Edge{
		int self_id = 0;
		pair <int, int> vert = {-1,-1};
	};
	
	struct Vert{
		int inf;
		vector<int> edges = {};
	};

	void set_vert_inf(int id, int inf) {
		vert_inf[id].inf = inf;
	}
	
	void set_vert(Vert v) {
		vert_inf.push_back(v);  
	}

	void set_vert_inf(vector<Vert> inf) {
		vert_inf = inf;
	}

	void set_edge(Edge edge) {
	 	edges.push_back(edge);
		vert_inf[edge.vert.first].edges.push_back(edge.self_id);
		vert_inf[edge.vert.second].edges.push_back(edge.self_id);
	}

	Graph (vector<Edge> newedges, vector<int> inf) {
		for (auto v: inf)
			set_vert({v,{}}); 
		for (auto edge: newedges)
			set_edge(edge);
	}

	void print() {
		for (auto edge: edges)
			cout<< edge.self_id << " " << vert_inf[edge.vert.first].inf << " "<< vert_inf[edge.vert.second].inf <<"\n";
	}

	void printjson() {
		std::ofstream out("graph.json");
		std::streambuf *old = cout.rdbuf();
		cout.rdbuf(out.rdbuf());
		cout<< "{\"vertices\":[";
		for (int j = 0; j < vert_inf.size(); j++) {
			Vert v = vert_inf[j];
			cout<< "{\"id\":"<< v.inf << ",\"edge_ids\":[";
			for (int i = 0; i < v.edges.size() - 1; i++) 
				cout << v.edges[i] <<",";
			cout << v.edges[v.edges.size()-1] << "]}";
			if (j!= vert_inf.size()-1) cout << ",";
		}
		cout<<"],\"edges\":[";
		for (int j = 0; j < edges.size(); j++) {
			Edge e = edges[j];
			cout<< "{\"id\":" << e.self_id << ",\"vertex_ids\":[" << e.vert.first <<"," << e.vert.second << "]}";
			if (j!= edges.size()-1) cout << ",";
		}
		cout<<"]}";
		cout.rdbuf(old);
	}

private: 
	vector <Edge> edges = {};
	vector <Vert> vert_inf = {};
};

int main() {
	vector <int> info(LEN);
	for (int i = 0; i < LEN; i++)
		info[i] = i;
	vector <Graph::Edge> edges = {{0,{0,1}}, {1,{0,2}}, {2,{0,3}}, {3,{1,4}}, {4,{1,5}}, {5,{1,6}}, {6,{2,7}},
		{7,{2,8}}, {8,{3,9}}, {9,{4,10}}, {10,{5,10}}, {11,{6,10}}, {12,{7,11}}, {13,{8,11}}, {14,{9,11}}, {15,{10,13}}, {16,{11,13}}, {17,{12,13}}};
	Graph g(edges, info);
	g.printjson();
	return 0;
}
