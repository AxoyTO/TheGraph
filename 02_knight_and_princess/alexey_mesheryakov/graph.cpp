#include <iostream>
#include <fstream>
#include <vector>
#include <utility>
#include <string>  
#include <sstream> 

constexpr int LEN = 14;

using std::pair;
using std::vector;
using std::cout;
using EdgeId = int;

class Graph {
public:
	struct Edge{
		EdgeId id = 0;
		pair <EdgeId, EdgeId> vert = {-1,-1};
	};
	
	struct Vert{
		int id;
		vector<EdgeId> edges = {};
	};

	
	void set_vert(Vert v) {
		vert_id_.push_back(v);  
	}

	void set_vert_id(vector<Vert> id) {
		vert_id_ = id;
	}

	void set_edge(Edge edge) {
	 	edges_.push_back(edge);
		vert_id_[edge.vert.first].edges.push_back(edge.id);
		vert_id_[edge.vert.second].edges.push_back(edge.id);
	}
	
	//Graph(){};

	void set_params(const vector<Edge>& new_edges, const vector<int>& vert_ids) {
		for (const auto& id: vert_ids)
			set_vert({id,{}}); 
		for (const auto& edge: new_edges)
			set_edge(edge);
	}

	std::string to_string() const{
		std::stringstream buffer;
		for (auto edge: edges_)
			buffer << edge.id << " " << vert_id_[edge.vert.first].id << " "<< vert_id_[edge.vert.second].id <<"\n";
		return buffer.str();
	}

	std::string to_json() const{
		std::stringstream buffer;
		buffer<< "{\"vertices\":[";
		for (int j = 0; j < vert_id_.size(); j++) {
			Vert v = vert_id_[j];
			buffer<< "{\"id\":"<< v.id << ",\"edge_ids\":[";
			for (int i = 0; i < v.edges.size() - 1; i++) 
				buffer << v.edges[i] <<",";
			buffer << v.edges[v.edges.size()-1] << "]}";
			if (j!= vert_id_.size()-1) buffer << ",";
		}
		buffer<<"],\"edges\":[";
		for (int j = 0; j < edges_.size(); j++) {
			Edge e = edges_[j];
			buffer<< "{\"id\":" << e.id << ",\"vertex_ids\":[" << e.vert.first <<"," << e.vert.second << "]}";
			if (j!= edges_.size()-1) buffer << ",";
		}
		buffer<<"]}\n";
		return buffer.str();
	}

private: 
	vector <Edge> edges_ = {};
	vector <Vert> vert_id_ = {};
};

int main() {
	vector <int> info(LEN);
	for (int i = 0; i < LEN; i++)
		info[i] = i;
	vector <Graph::Edge> edges = {{0,{0,1}}, {1,{0,2}}, {2,{0,3}}, {3,{1,4}}, {4,{1,5}}, {5,{1,6}}, {6,{2,7}},
		{7,{2,8}}, {8,{3,9}}, {9,{4,10}}, {10,{5,10}}, {11,{6,10}}, {12,{7,11}}, {13,{8,11}}, {14,{9,12}}, {15,{10,13}}, {16,{11,13}}, {17,{12,13}}};
	Graph graph;
	graph.set_params(edges, info);
	cout << graph.to_string();
	std::ofstream out("graph.json");
	std::streambuf *old = cout.rdbuf();
	cout.rdbuf(out.rdbuf());
	cout<< graph.to_json();
	cout.rdbuf(old);
	return 0;
}
