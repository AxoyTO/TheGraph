#include <iostream>
#include <vector>
#include <string>
#include <tuple>
#include <set>
#include <fstream>

using std::cout;
using std::endl;
using std::get;
using std::to_string;
using std::vector;

using NodeId = int;
using EdgeId = int;

//Tuple of <int,int> for realization of vector of vectors of <int,int>
using dest_edge_id = std::tuple<int, int>;

//Structure of nodes(vertices)
struct Node
{
public:
	NodeId id = 0;

	std::string to_JSON(const vector<int> &edge_ids, bool end_flag) const
	{
		std::string str = "";
		static int st_counter = 0;

		if (!(st_counter++))
			str += "{\n\"vertices\": [\n";
		str += "\t{ \"id\": " + to_string(id) + ", \"edge_ids\": [";
		for (int i = 0; i < edge_ids.size(); i++)
		{
			str += to_string(edge_ids[i]);
			if (i + 1 == edge_ids.size())
			{
				str += "] }";
				if (!end_flag)
					str += ",\n";
				else
					str += "\n  ],\n";
			}
			else
				str += ", ";
		}
		return str;
	}
};

//Structure of the edge including the source and destination nodes in it
struct Edge
{
public:
	EdgeId id = 0;
	Node src_node;
	Node dest_node;
	Edge(int src_id, int dest_id, int arg_id)
	{
		src_node.id = src_id;
		dest_node.id = dest_id;
		id = arg_id;
	}

	std::string to_JSON(int end_flag) const
	{
		std::string str = "";
		static int st_counter = 0;
		if (!(st_counter++))
			str += "\"edges\": [\n";
		str += "\t{ \"id\": " + to_string(id) + ", \"vertex_ids\": [" + to_string(src_node.id) + ", " + to_string(dest_node.id) + "] }";
		if (!end_flag)
			str += ",\n";
		else
			str += "\n  ]\n}\n";
		return str;
	}
};

class Graph
{
private:
	int total_nodes_;
	int total_edges_;
	vector<std::pair<int, int>> edges_src_dest; //vector of pairs to keep data of source and destination vertices of an edge
	vector<vector<dest_edge_id>> graph_vec_;

public:
	Graph()
	{
		generateVertices();
	}

	void generateVertices()
	{
		const vector<Edge> edges =
			{{0, 1, 0}, {0, 2, 1}, {0, 3, 2}, {1, 4, 3}, {1, 5, 4}, {1, 6, 5}, {2, 7, 6}, {2, 8, 7}, {3, 9, 8}, {4, 10, 9}, {5, 10, 10}, {6, 10, 11}, {7, 11, 12}, {8, 11, 13}, {9, 12, 14}, {10, 13, 15}, {11, 13, 16}, {12, 13, 17}};
		//using set to exclude the repeating source nodes(the ones with more than 1 edge connected)
		std::set<int> nodes_set;
		for (int i = 0; i < edges.size(); i++)
		{
			nodes_set.insert(edges[i].src_node.id);
		}
		total_nodes_ = nodes_set.size();
		total_edges_ = edges.size();
		graph_vec_.resize(total_nodes_ * 2);
		for (const auto &edge : edges)
		{
			const int src_id = edge.src_node.id;
			if (src_id >= total_nodes_)
				continue;
			const int dest_id = edge.dest_node.id;
			const int edge_id = edge.id;

			edges_src_dest.push_back(std::make_pair(src_id, dest_id));

			graph_vec_[src_id].push_back(std::make_tuple(dest_id, edge_id));
			graph_vec_[dest_id].push_back(std::make_tuple(src_id, edge_id));
		}
	}

	std::string to_JSON() const
	{
		Node src = {};
		Edge e(0, 0, 0);
		vector<int> edge_ids;
		bool end_flag = 0;
		std::string str;
		for (int i = 0; i <= total_nodes_; i++)
		{
			src.id = i;
			for (int j = 0; j < graph_vec_[i].size(); j++)
			{
				edge_ids.push_back(get<1>(graph_vec_[i][j]));
			}
			if (i == total_nodes_)
				end_flag = 1;
			str += src.to_JSON(edge_ids, end_flag);
			edge_ids.clear();
		}
		end_flag = 0;

		for (int i = 0; i < total_edges_; i++)
		{
			if (i + 1 == total_edges_)
				end_flag = 1;
			e.id = i;
			e.src_node.id = edges_src_dest[i].first;
			e.dest_node.id = edges_src_dest[i].second;
			str += e.to_JSON(end_flag);
		}
		return str;
	}
};

int main()
{
	Graph graph;
	std::ofstream file("graph.json", std::ios::out);
	if (!file.is_open())
		std::cerr << "Error opening the file graph.json!" << endl;
	else
	{
		std::string str = graph.to_JSON();
		file << str;
	}
	return 0;
}
