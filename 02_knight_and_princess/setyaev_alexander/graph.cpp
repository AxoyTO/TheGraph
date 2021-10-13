#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>

struct Vertex{
	int id = 0;
	std::vector <int> edges;
};

struct Edge{
	int id = 0;
	int start_id = 0;
	int end_id = 0;
};

class Graph{
public:
	void add_vertex(int i);
	void add_edge(int i, int first, int second);
	void vertex_input();
	void edge_input();
	void init();
	std::string json_output() const;
private:
	std::vector<Vertex> vertices;
	std::vector<Edge> edges;
	bool is_vertex_exists(int i) const;
	bool is_edge_exists(int i) const;
};

void Graph::add_vertex(int i){
	Vertex tmp;
	tmp.id = i;
	vertices.push_back(tmp);
}

void json_to_file(std::string str);

void Graph::add_edge(int i, int first, int second){
	Edge tmp;
	tmp.id = i;
	tmp.start_id = first;
	tmp.end_id = second;
	for (int j = 0; j < vertices.size(); ++ j){
		if (vertices[j].id == first){
			vertices[j].edges.push_back(i);
		}
		if (vertices[j].id == second){
			vertices[j].edges.push_back(i);
		}
	}
	edges.push_back(tmp);
}

void Graph::vertex_input(){
	int i = 0;
	std::cout << "Enter vertex id. Print non-number to exit" << std::endl;
	while (std::cin >> i){
		if (!is_vertex_exists(i)){
			add_vertex(i);
			std::cout << "Vertex " << i << " added" << std::endl;
		}
		else{
			std::cout << "Allready exist" << std::endl;
		}
	}
}

void Graph::edge_input(){
	int i1 = 0, i2 = 0, i = 0;
	std::cout << "Enter start vertex id. Then enter end vertex id.\n At least enter edge id. Print non-number to exit." << std::endl;
	while (std::cin >> i1 >> i2 >> i){
		if (!is_vertex_exists(i1)){
			add_vertex(i1);
			std::cout << "Vertex " << i1 << " added" << std::endl;
		}
		if (!is_vertex_exists(i2)){
			add_vertex(i2);
			std::cout << "Vertex " << i2 << " added" << std::endl;
		}
		if (!is_edge_exists(i)){
			add_edge(i, i1, i2);
			std::cout << "Edge " << i << " added: " << i1 << "<->" << i2 << std::endl;
		}
		else{
			std::cout << "Edge " << i << " allready exist" << std::endl;
		}
	}
}

void Graph::init(){
	for (int i = 0; i < 14; ++i){
		add_vertex(i);
	}
	add_edge(0, 0, 1);
	add_edge(1, 0, 2);
	add_edge(2, 0, 3);
	add_edge(3, 1, 4);
	add_edge(4, 1, 5);
	add_edge(5, 1, 6);
	add_edge(6, 2, 7);
	add_edge(7, 2, 8);
	add_edge(8, 3, 9);
	add_edge(9, 4, 10);
	add_edge(10, 5, 10);
	add_edge(11, 6, 10);
	add_edge(12, 7, 11);
	add_edge(13, 8, 11);
	add_edge(14, 9, 12);
	add_edge(15, 10, 13);
	add_edge(16, 11, 13);
	add_edge(17, 12, 13);
}

std::string Graph::json_output() const{
	
	std::stringstream json;
	
	json << "{" << std::endl << "	\"vertices\": [" << std::endl;
	
	for (int i = 0; i < vertices.size(); ++i){
		json << "		{\n			\"id\": " << vertices[i].id << "," << std::endl << "			\"edge_ids\": [";
		for (int j = 0; j < vertices[i].edges.size(); ++j){
			json << vertices[i].edges[j];
			if (j < vertices[i].edges.size()-1){
				json << ", ";
			}
			else{
				json << "]" << std::endl;
			}
		}
		json << "		}";
		if (i < vertices.size() - 1){
			json << ",\n";
		}
		else{
			json << "\n	],\n	\"edges\": [\n";
		}
	}
	for (int i = 0; i < edges.size(); ++i){
		json << "		{\n			\"id\": " << edges[i].id << ",\n			\"vertex_ids\": [" << edges[i].start_id << ", " << edges[i].end_id << "]\n		}";
		if (i < edges.size() - 1){
			json << ",\n";
		}
		else{
			json <<  "\n	]\n}";
		}
	}
	
	return json.str();
}

void json_to_file(std::string str){
	std::ofstream json;
	json.open("graph.json");
	if (!json.is_open()){
		return;
	}
	json << str;
	json.close();
}


bool Graph::is_vertex_exists(int i) const{
	for (int j = 0; j < vertices.size(); ++j){
		if (vertices[j].id == i){
			return true;
		}
	}
	return false;
}

bool Graph::is_edge_exists(int i) const{
	for (int j = 0; j < edges.size(); ++j){
		if (edges[j].id == i){
			return true;
		}
	}
	return false;
}

int main(){
	
	Graph g1;
	//g1.edge_input();
	g1.init();
	std::string tmp = g1.json_output();
	json_to_file(tmp);
	
	return 0;
}
