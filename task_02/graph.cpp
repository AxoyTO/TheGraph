#include <iostream>
#include <fstream>
#include <vector>

struct Vertex{
	int id = 0;
	std::vector <int> e_ids;
};

struct Edge{
	int id = 0;
	int start_id = 0;
	int end_id = 0;
};

class Graph{
private:
	std::vector<Vertex> v_list;
	std::vector<Edge> e_list;
	bool find_vertex(int i);
	bool find_edge(int i);
public:
	//Graph();
	void add_vertex(int i);
	void add_edge(int i, int first, int second);
	void vertex_input();
	void edge_input();
	void init();
	void json_output();
};

void Graph::add_vertex(int i){
	Vertex tmp;
	tmp.id = i;
	v_list.push_back(tmp);
}

void Graph::add_edge(int i, int first, int second){
	Edge tmp;
	tmp.id = i;
	tmp.start_id = first;
	tmp.end_id = second;
	for (int j = 0; j < v_list.size(); ++ j){
		if (v_list[j].id == first){
			v_list[j].e_ids.push_back(i);
		}
		if (v_list[j].id == second){
			v_list[j].e_ids.push_back(i);
		}
	}
	e_list.push_back(tmp);
}

void Graph::vertex_input(){
	int i = 0;
	std::cout << "Enter vertex id. Print non-number to exit" << std::endl;
	while (std::cin >> i){
		if (!find_vertex(i)){
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
		if (!find_vertex(i1)){
			add_vertex(i1);
			std::cout << "Vertex " << i1 << " added" << std::endl;
		}
		if (!find_vertex(i2)){
			add_vertex(i2);
			std::cout << "Vertex " << i2 << " added" << std::endl;
		}
		if (!find_edge(i)){
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

void Graph::json_output(){
	std::ofstream json;
	json.open("graph.json");
	if (!json.is_open()){
		return;
	}
	json << "{" << std::endl << "	\"vertices\": [" << std::endl;
	
	for (int i = 0; i < v_list.size(); ++i){
		json << "		{\n			\"id\": " << v_list[i].id << "," << std::endl << "			\"edge_ids\": [";
		for (int j = 0; j < v_list[i].e_ids.size(); ++j){
			json << v_list[i].e_ids[j];
			if (j < v_list[i].e_ids.size()-1){
				json << ", ";
			}
			else{
				json << "]" << std::endl;
			}
		}
		json << "		}";
		if (i < v_list.size() - 1){
			json << ",\n";
		}
		else{
			json << "\n	],\n	\"edges\": [\n";
		}
	}
	for (int i = 0; i < e_list.size(); ++i){
		json << "		{\n			\"id\": " << e_list[i].id << ",\n			\"vertex_ids\": [" << e_list[i].start_id << ", " << e_list[i].end_id << "]\n		}";
		if (i < e_list.size() - 1){
			json << ",\n";
		}
		else{
			json <<  "\n	]\n}";
		}
	}
}


bool Graph::find_vertex(int i){
	for (int j = 0; j < v_list.size(); ++j){
		if (v_list[j].id == i){
			return true;
		}
	}
	return false;
}

bool Graph::find_edge(int i){
	for (int j = 0; j < e_list.size(); ++j){
		if (e_list[j].id == i){
			return true;
		}
	}
	return false;
}

int main(){
	
	Graph g1;
	//g1.edge_input();
	g1.init();
	g1.json_output();
	
	return 0;
}
